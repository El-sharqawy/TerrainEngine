#include "stdafx.h"
#include "../../LibGL/source/shader.h"

#include "../../LibGL/source/screen.h"
#include "../../LibTerrain/source/midpoint_terrain.h"
#include "../../LibTerrain/source/object.h"
#include "../../LibTerrain/source/skybox.h"

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
#if defined(_DEBUG)
#pragma comment(lib, "Debug/assimp-vc143-mtd.lib")
#pragma comment(lib, "Debug/zlibstaticd.lib")
#pragma comment(lib, "Debug/meshoptimizer_mtd.lib")
#else
#pragma comment(lib, "Release/assimp-vc143-mt.lib")
#pragma comment(lib, "Release/zlibstatic.lib")
#pragma comment(lib, "Release/meshoptimizer.lib")
#endif

CMidPointTerrain* m_pTerrain;
CWindow* app;
std::unique_ptr<CMesh> pMesh;

bool bStuckCam = false;

static void InitTerrain()
{
	float WorldScale = 64.0f;
	float TextureScale = 1.0f;

	std::vector<std::string> TextureFilenames;
	TextureFilenames.emplace_back("resources/textures/rock02_2.jpg");
	TextureFilenames.emplace_back("resources/textures/rock01.jpg");
	TextureFilenames.emplace_back("resources/textures/tilable-IMG_0044-verydark.png");
	TextureFilenames.emplace_back("resources/textures/water.png");

	m_pTerrain->InitializeTerrain(WorldScale, TextureScale, TextureFilenames);

	GLint iSize = 2048;
	GLint iPatchSize = 32;
	float fMinHeight = 0.0f;
	float fMaxHeight = 500.0f;
	float Roughness = 1.0f;

	m_pTerrain->CreateMidPointTerrain(iSize, iPatchSize, Roughness, fMinHeight, fMaxHeight);
	SVector3Df LightDir(1.0f, -1.0f, 0.0f);
	m_pTerrain->SetLightDir(LightDir);

	m_pTerrain->SaveToFile("resources/terrain/heightmap.png");
}

static void RenderSceneNew(const float deltaTime)
{
	CCameraManager::Instance().GetCurrentCamera()->OnRender();

	if (bStuckCam)
	{
		SVector3Df vNewPos = m_pTerrain->ConstrainCameraToTerrain();
		app->GetCamera()->SetPosition(vNewPos);
	}

	m_pTerrain->Render();

	double mouseX, mouseY;
	GLint winW, winH;
	glfwGetCursorPos(app->GetWindow(), &mouseX, &mouseY);
	glfwGetWindowSize(app->GetWindow(), &winW, &winH);

	app->GetScreen()->SetCursorPosition((GLint)mouseX, (GLint)mouseY, winW, winH);
}

int main()
{
#ifdef _DEBUG
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	SRANDOM;

	app = new CWindow();
	
	if (!app->InitializeWindow("Terrain Engine", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT))
	{
		sys_err("Failed to Create the Application");
		return (EXIT_FAILURE);
	}

	app->SetWindowIcon("resources/icon/terrain.png");

	float deltaTime = 0.0f; // Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	m_pTerrain = new CMidPointTerrain();

	InitTerrain();

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEBUG_OUTPUT);

	CScreen* screen = new CScreen;
	app->SetScreen(screen);

	int Major, Minor, Rev;

	glfwGetVersion(&Major, &Minor, &Rev);
	printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);
	
	app->GetScreen()->SetTerrain(m_pTerrain);

	app->SetMidPointTerrain(m_pTerrain);

	CUserInterface UI(app);
	CSkyBox skyBox(app);

	SVector3Df v3FogColor(0.5f, 0.6f, 0.7f);
	SVector3Df v3LightColor(255.0f, 255.0f, 230.0f);
	v3LightColor /= 255.0f;

	//FrameBufferObject SceneFBO(app->GetWidth(), app->GetHeight());

	SVector3Df v3LightPosition{}, v3Seed{};
	SVector3Df v3LightDir(-0.5f, 0.5f, 1.0f);

	//Every scene object need these informations to be rendered
	SSceneElements scene;
	scene.v3LightPos = v3LightPosition;
	scene.v3LightColor = v3LightColor;
	scene.v3LightDir = v3LightDir;
	scene.v3FogColor = v3FogColor;
	scene.v3Seed = v3Seed;
	scene.mProjMatrix = app->GetCamera()->GetProjectionMat();
	scene.pCamera = app->GetCamera();
	//scene.pSceneFBO = skyBox.GetFBO();

	CObject::pScene = &scene;

	UI.AddObject(&skyBox);
	UI.AddObject(m_pTerrain);

	app->SetFrameBufferObject(skyBox.GetFBO());

	while (app->WindowLoop())
	{
		scene.v3LightDir.normalize();
		scene.v3LightPos = scene.v3LightDir * 1e6f + app->GetCamera()->GetPosition();

		float frametime = 1 / ImGui::GetIO().Framerate;
		app->ProcessInput(frametime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, scene.v3FogColor);

		UI.Update();
		skyBox.Update();
		app->GetScreen()->Update();

		//UnbindCurrentFrameBuffer(app);

		// Render the skybox first with depth testing disabled
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

		RenderSceneNew(frametime);

		//SceneFBO.UnBind();

		// Render UI on top
		UI.Render();

		app->WindowSwapAndBufferEvents();
	}

	delete screen;
	screen = nullptr;

	delete m_pTerrain;
	m_pTerrain = nullptr;

	delete app;
	app = nullptr;

	return (EXIT_SUCCESS);
}

