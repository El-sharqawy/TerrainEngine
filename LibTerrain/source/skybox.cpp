#include "stdafx.h"
#include "skybox.h"
#include "../../LibImageUI/imgui.h"
#include "../../LibImageUI/imgui_impl_glfw.h"
#include "../../LibImageUI/imgui_impl_opengl3.h"

CSkyBox::CSkyBox(CWindow* pWindow)
{
	m_v3SkyColorTop = SVector3Df(0.5f, 0.7f, 0.8f) * 1.05f;
	m_v3SkyColorBottom = SVector3Df(0.9f, 0.9f, 0.95f);

	m_pSkyboxScreenSpace = new CScreenSpaceShader("shaders/skybox.frag", "SkyBoxShader");
	m_pSkyBoxNewFBO = new CFrameBuffer();
	m_pSkyBoxNewFBO->InitSkyBox(pWindow->GetWidth(), pWindow->GetHeight());

	m_pWindow = pWindow;

	m_bManualOverride = true;

	SunsetPresetTwo();
	DefaultPreset();
}

void CSkyBox::Render()
{
    SSceneElements* scene = CObject::pScene;
    if (!scene)
    {
        sys_err("CSkyBox::Render: Scene Is NULL!!");
        return;
    }

	m_pSkyBoxNewFBO->BindForWriting();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE); // Ensure culling doesn’t interfere
	glDisable(GL_BLEND); // Disable blending to avoid transparency issues

    // Set up shader
    m_pSkyboxScreenSpace->GetShader().Use();
    m_pSkyboxScreenSpace->GetShader().setMat4("m4Inv_proj", scene->pCamera->GetProjectionMat().Inverse());
    m_pSkyboxScreenSpace->GetShader().setMat4("m4Inv_view", scene->pCamera->GetViewMatrix().Inverse());
    m_pSkyboxScreenSpace->GetShader().setVec2("v2Resolution", SVector2Df(static_cast<float>(m_pWindow->GetWidth()), static_cast<float>(m_pWindow->GetHeight())));

	SVector3Df v3LightDir = (scene->v3LightPos - scene->pCamera->GetPosition());
	v3LightDir.normalize();

    m_pSkyboxScreenSpace->GetShader().setVec3("v3LightDir", v3LightDir);
    m_pSkyboxScreenSpace->GetShader().setVec3("v3SkyColorTop", m_v3SkyColorTop);
    m_pSkyboxScreenSpace->GetShader().setVec3("v3SkyColorBottom", m_v3SkyColorBottom);

	m_pSkyboxScreenSpace->GetShader().setInt("screenTexture", 0);  // Set the uniform to use texture unit 0

	// Render to framebuffer
	m_pSkyboxScreenSpace->Render();

	// Blit the FBO’s color buffer into the default framebuffer
	m_pSkyBoxNewFBO->BindToDefaultBuffer();

	// Go back to default for further rendering
	m_pSkyBoxNewFBO->UnBindWriting();
}

void CSkyBox::SetGUI()
{
	ImGui::Begin("SkyBox Controls: ");
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Sky colors controls");
	if (ImGui::ColorEdit3("Sky Top Color", (float*)&m_v3SkyColorTop))		// Edit 3 floats representing a color
	{
		m_bManualOverride = true;
	}
	if (ImGui::ColorEdit3("Sky Bottom Color", (float*)&m_v3SkyColorBottom))	// Edit 3 floats representing a color
	{
		m_bManualOverride = true;
	}

	// A button to return to AUTO Original Color Mode
	if (ImGui::Button("Reset to Auto"))
		m_bManualOverride = false;

	ImGui::End();
}

void CSkyBox::Update()
{
	// if the ColorEdit3 widget is active, skip the mix
	if (!m_bManualOverride)
	{
		auto sigmoid = [](float fVal)
			{
				return (1 / (1.0f + std::exp(8.0f - fVal * 40.0f)));
			};

		MixSkyColorPresets(sigmoid(pScene->v3LightDir.y), m_sHighSunPreset, m_sPresetSunset);
	}
}

TColorPreset CSkyBox::DefaultPreset()
{
	TColorPreset colPreset{};

	colPreset.v3CloudColorBottom = (SVector3Df(65.0f, 70.0f, 80.0f) * (1.5f / 255.0f));
	colPreset.v3SkyColorTop = SVector3Df(0.5f, 0.7f, 0.8f) * 1.05f;
	colPreset.v3SkyColorBottom = SVector3Df(0.9f, 0.9f, 0.95f);
	colPreset.v3LightColor = SVector3Df(255.0f, 255.0f, 230.0f) / 255.0f;
	colPreset.v3FogColor = SVector3Df(0.5f, 0.6f, 0.7f);

	m_sHighSunPreset = colPreset;

	return (colPreset);

}

TColorPreset CSkyBox::SunsetPreset()
{
	TColorPreset colPreset{};

	colPreset.v3CloudColorBottom = SVector3Df(89.0f, 96.0f, 109.0f) / 255.0f;
	colPreset.v3SkyColorTop = SVector3Df(177.0f, 174.0f, 119.0f) / 255.0f;
	colPreset.v3SkyColorBottom = SVector3Df(234.0f, 125.0f, 125.0f) / 255.0f;
	colPreset.v3LightColor = SVector3Df(255.0f, 171.0f, 125.0f) / 255.0f;
	colPreset.v3FogColor = SVector3Df(85.0f, 97.0f, 120.0f) / 255.0f;

	m_sPresetSunset = colPreset;

	return (colPreset);
}

TColorPreset CSkyBox::SunsetPresetTwo()
{
	TColorPreset colPreset{};

	colPreset.v3CloudColorBottom = SVector3Df(97.0f, 98.0f, 120.0f) / 255.0f;
	colPreset.v3SkyColorTop = SVector3Df(133.0f, 158.0f, 214.0f) / 255.0f;
	colPreset.v3SkyColorBottom = SVector3Df(241.0f, 161.0f, 161.0f) / 255.0f;
	colPreset.v3LightColor = SVector3Df(255.0f, 201.0f, 201.0f) / 255.0f;
	colPreset.v3FogColor = SVector3Df(128.0f, 153.0f, 179.0f) / 255.0f;

	m_sPresetSunset = colPreset;

	return (colPreset);
}

void CSkyBox::MixSkyColorPresets(GLfloat fVal, const TColorPreset& p1, const TColorPreset& p2)
{
	float fA = std::min(std::max(fVal, 0.0f), 1.0f);
	float fFactor = 1.0f - fA;

	m_v3SkyColorTop = p1.v3SkyColorTop * fA + p2.v3SkyColorTop * fFactor;
	m_v3SkyColorBottom = p1.v3SkyColorBottom * fA + p2.v3SkyColorBottom * fFactor;

	pScene->v3LightColor = p1.v3LightColor * fA + p2.v3LightColor * fFactor;
	pScene->v3FogColor = p1.v3FogColor * fA + p2.v3FogColor * fFactor;
}

CSkyBox::~CSkyBox()
{
	delete m_pSkyboxScreenSpace;
	if (m_pSkyBoxNewFBO)
	{
		delete m_pSkyBoxNewFBO;
	}
}

