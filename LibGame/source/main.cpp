#include "stdafx.h"
#include "../../LibGL/source/shader.h"

#include "../../LibGL/source/screen.h"
#include "../../LibGL/source/mesh.h"
#include "../../LibTerrain/source/terrain.h"
#include "../../LibTerrain/source/object.h"
#include "../../LibTerrain/source/skybox.h"
#include "../../LibTerrain/source/clouds_object.h"

#include "userinterface.h"

// IMGUI
#include "../../LibImageUI/imgui.h"
#include "../../LibImageUI/imgui_impl_glfw.h"
#include "../../LibImageUI/imgui_impl_opengl3.h"

#include <memory>
#ifdef _DEBUG
#include <crtdbg.h>
#endif

#pragma comment(lib, "glfw3.lib")

#if defined(ENABLE_USE_PYTHON)
#include <python/Python.h>
#endif

#if defined(_DEBUG)
#pragma comment(lib, "Debug/assimp-vc143-mtd.lib")
#pragma comment(lib, "Debug/zlibstaticd.lib")
#pragma comment(lib, "Debug/meshoptimizer_mtd.lib")
#else
#pragma comment(lib, "Release/assimp-vc143-mt.lib")
#pragma comment(lib, "Release/zlibstatic.lib")
#pragma comment(lib, "Release/meshoptimizer.lib")
#endif

CBaseTerrain* m_pTerrain;
CWindow* app;
std::unique_ptr<CMesh> pMesh;
std::unique_ptr<CShader> pMeshShader;
std::vector<CTexture*> vTerrainTextures;
GLuint terrainHandlesSSBO;
std::vector<GLuint64> textureHandles;
CTerrainTextureSet* textureset;

static void SetPythonMood(bool bActive)
{
	if (bActive)
	{
		Py_Initialize();
		if (!Py_IsInitialized())
		{
			sys_err("Failed to Initialize Python");
			std::exit(1);
		}
		sys_log("Python Version: %s Initialized.", Py_GetVersion());
	}
	else
	{
		if (Py_IsInitialized())
		{
			Py_Finalize();
		}
	}
}

CWorldTranslation* worldTransform = nullptr;

static void LoadTerrainTextures()
{
	std::vector<std::string> paths = { "resources/textures/rock02_2.jpg", "resources/textures/rock01.jpg", "resources/textures/tilable-IMG_0044-verydark.png", "resources/textures/water.png" };

	for (auto& path : paths)
	{
		CTexture* tex = new CTexture(path, GL_TEXTURE_2D);
		if (!tex->Load())
		{
			sys_err("Failed to load texture %s", path.c_str());
			continue;
		}
		//tex.GenerateMipmap();  // Required for bindless
		tex->MakeResident();
		vTerrainTextures.push_back(tex);
	}
}

static void FinalizeBindlessSetup()
{
	// Collect handles from loaded textures
	for (auto& tex : vTerrainTextures)
	{
		textureHandles.push_back(tex->GetHandle()); // Assuming CTexture has GetHandle()
		sys_err("FinalizeBindlessSetup: Added Handle: %zu", tex->GetHandle());
	}

	// Create and fill SSBO
	glGenBuffers(1, &terrainHandlesSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, terrainHandlesSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, textureHandles.size() * sizeof(GLuint64), textureHandles.data(), GL_STATIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, terrainHandlesSSBO);

	// After creating SSBO
	GLuint64* handles = static_cast<GLuint64*>(
		glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));

	for (size_t i = 0; i < textureHandles.size(); ++i)
	{
		if (handles[i] != vTerrainTextures[i]->GetHandle())
		{
			std::cerr << "SSBO handle mismatch at index " << i << "\n";
		}
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

static void InitTerrain()
{
	GLint iMaxTextures, iMaxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &iMaxTextures);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &iMaxTextureSize);

	float WorldScale = 1.0f;
	float TextureScale = 1.0f;

	GLint iSize = 513;
	GLint iPatchSize = 33;

	CBaseTerrain::Instance().InitializeTerrain(iSize, iPatchSize, WorldScale, TextureScale);

	textureset = new CTerrainTextureSet();
	textureset->Load("resources/terrain/textureset.json");

	CBaseTerrain::Instance().SetTextureSet(textureset);

	CBaseTerrain::Instance().DoBindlesslyTexturesSetup();

	CBaseTerrain::Instance().bRender = true;

	pMeshShader = std::make_unique<CShader>("ModelShader");

	pMesh = std::make_unique<CMesh>();
	if (!pMesh->LoadMesh("resources/models/test1.obj"))
	{
		sys_err("Failed to Load Model SK_Warrior_4_1.fbx");
	}

	worldTransform = &pMesh->GetWorldTranslation();

	worldTransform->SetScale(0.1f);
	worldTransform->Rotate(0.0f, 0.0f, 0.0f);

	// TODO: Initialize shaderProgram with a basic line shader
	pMeshShader->AttachShader("shaders/model_shader.vert");
	pMeshShader->AttachShader("shaders/model_shader.frag");
	pMeshShader->LinkPrograms();


	//m_pTerrain->SaveToFile("resources/terrain/heightmap.png");
}

bool bStuckCam = false;

static void RenderSceneNew(const float deltaTime)
{
	CCameraManager::Instance().GetCurrentCamera()->OnRender();

	if (bStuckCam)
	{
		SVector3Df vNewPos = m_pTerrain->ConstrainCameraToTerrain();
		//app->GetCamera()->SetPosition(vNewPos);
	}


	worldTransform->SetPosition(CScreen::Instance().GetIntersectionPoint());
	CMatrix4Df World = worldTransform->GetMatrix();

	CMatrix4Df View = CCameraManager::Instance().GetCurrentCamera()->GetMatrix();

	CMatrix4Df Projection{};
	Projection.InitPersProjTransform(CCameraManager::Instance().GetCurrentCamera()->GetPersProjInfo());

	// Bind SSBO once per frame
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, terrainHandlesSSBO);
	CBaseTerrain::Instance().Render();
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);

	CMatrix4Df WVP = Projection * View * World;
	pMeshShader->Use();
	pMeshShader->setMat4("gWVP", WVP);


	pMesh->Render();

	double mouseX, mouseY;
	GLint winW, winH;
	glfwGetCursorPos(app->GetWindow(), &mouseX, &mouseY);
	glfwGetWindowSize(app->GetWindow(), &winW, &winH);

	CScreen::Instance().SetCursorPosition((GLint)mouseX, (GLint)mouseY, winW, winH);
}

int main()
{
#ifdef _DEBUG
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	SRANDOM();

	app = new CWindow();
	
	if (!app->InitializeWindow("Terrain Engine", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT))
	{
		sys_err("Failed to Create the Application");
		return (EXIT_FAILURE);
	}

	m_pTerrain = new CBaseTerrain();

	InitTerrain();

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEBUG_OUTPUT);

	CScreen* screen = new CScreen;

	CUserInterface UI(app);

	//Every scene object need these informations to be rendered
	SVector3Df v3FogColor(0.5f, 0.6f, 0.7f);
	SVector3Df v3LightColor(255.0f, 255.0f, 230.0f);
	v3LightColor /= 255.0f;

	CFrameBuffer* SceneFBO = new CFrameBuffer();
	SceneFBO->Init(app->GetWidth(), app->GetHeight());

	SVector3Df v3LightPosition{}, v3Seed{};
	SVector3Df v3LightDir(-0.5f, 0.5f, 1.0f);

	SSceneElements scene;
	scene.v3LightPos = v3LightPosition;
	scene.v3LightColor = v3LightColor;
	scene.v3LightDir = v3LightDir;
	scene.v3FogColor = v3FogColor;
	scene.v3Seed = v3Seed;
	scene.mProjMatrix = app->GetCamera()->GetProjectionMat();
	scene.pCamera = app->GetCamera();
	scene.pSceneFBO = SceneFBO;

	CObject::pScene = &scene;

	CSkyBox skyBox(app);
	//CCloudsObject cloudsModel(&scene, &skyBox);


	UI.AddObject(&skyBox);
	//UI.AddObject(&cloudsModel);
	UI.AddObject(m_pTerrain);

	app->SetFrameBuffer(SceneFBO);

	while (app->WindowLoop())
	{
		scene.v3LightDir.normalize();
		scene.v3LightPos = scene.v3LightDir * 1e6f + app->GetCamera()->GetPosition();

		float frametime = 1 / ImGui::GetIO().Framerate;
		app->ProcessInput(frametime);

		app->GetFrameBuffer()->BindForWriting();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, scene.v3FogColor);

		UI.Update(); // will call Update for all Models!

		// Render the skybox first with depth testing disabled, with it's own FBO that is writing to default framebuffer
		skyBox.Render();

		// Render the terrain with depth testing enabled
		if (scene.bWireFrame)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		CScreen::Instance().Update();

		app->GetFrameBuffer()->BindForWriting();
		RenderSceneNew(frametime);
		app->GetFrameBuffer()->UnBindWriting();

		// Render UI on top
		UI.Render();

		app->WindowSwapAndBufferEvents();
	}

	for (auto& tex : vTerrainTextures)
	{
		tex->MakeNonResident();
		tex->Destroy();
	}

	safe_delete(textureset);
	safe_delete(SceneFBO);
	safe_delete(screen);
	safe_delete(m_pTerrain);
	safe_delete(app);


	return (EXIT_SUCCESS);
}

