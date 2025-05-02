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
	SSceneElements* scene = CObject::pScene;
	if (!scene)
	{
		sys_err("CSkyBox::Render: Scene Is NULL!!");
		return;
	}

	m_pSkyBoxNew = scene->pSceneFBO;

	m_pWindow = pWindow;

	m_bManualOverride = true;
	m_bIsNight = false;

	m_fStarDensity = m_fStarBrightness = 1.0f;

	m_fSunSizeDay = 0.9999f;    // bigger sun
	m_fSunSizeNight = 0.9990f;  // smaller sun
	m_fSunCoreIntensity = 2.0f;

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
	m_pSkyBoxNew->BindForWriting();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

	float currentTime = static_cast<float>(glfwGetTime());
	m_pSkyboxScreenSpace->GetShader().setFloat("fTime", currentTime);
	m_pSkyboxScreenSpace->GetShader().setBool("bIsNight", m_bIsNight);
	m_pSkyboxScreenSpace->GetShader().setFloat("fStarDensity", m_fStarDensity); // [0.5 - 3.0]
	m_pSkyboxScreenSpace->GetShader().setFloat("fStarBrightness", m_fStarBrightness); // [0.1 - 2.0]
	m_pSkyboxScreenSpace->GetShader().setFloat("fSunSizeNight", m_fSunSizeNight); // [0.9990 - 0.9999]
	m_pSkyboxScreenSpace->GetShader().setFloat("fSunSizeDay", m_fSunSizeDay); // [0.9990 - 0.9999]
	m_pSkyboxScreenSpace->GetShader().setFloat("fSunCoreIntensity", m_fSunCoreIntensity); // [0.1 - 3.0]

	// Render to framebuffer
	m_pSkyboxScreenSpace->Render();

	// Blit the FBO’s color buffer into the default framebuffer
	//m_pSkyBoxNew->BindToDefaultBuffer();

	// Go back to default for further rendering
	m_pSkyBoxNew->UnBindWriting();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); // Disable blending to avoid transparency issues
}

void CSkyBox::SetGUI()
{
	if (ImGui::ColorEdit3("Sky Top Color", (float*)&m_v3SkyColorTop))		// Edit 3 floats representing a color
	{
		m_bManualOverride = true;
	}
	if (ImGui::ColorEdit3("Sky Bottom Color", (float*)&m_v3SkyColorBottom))	// Edit 3 floats representing a color
	{
		m_bManualOverride = true;
	}

	bool changed = ImGui::Checkbox("Night mode", &m_bIsNight);

	if (changed)
	{
		if (m_bIsNight)
		{
			m_v3SkyColorTop = SVector3Df(14, 31, 82) / 255.0f;
			m_v3SkyColorBottom = SVector3Df(1, 16, 73) / 255.0f;
		}
		else
		{
			m_v3SkyColorTop = SVector3Df(0.5f, 0.7f, 0.8f) * 1.05f;
			m_v3SkyColorBottom = SVector3Df(0.9f, 0.9f, 0.95f);
		}

	}

	if (m_bIsNight)
	{
		ImGui::SliderFloat("Star Density", &m_fStarDensity, 0.5f, 3.0f);
		ImGui::SliderFloat("Star Brightness", &m_fStarBrightness, 0.1f, 2.0f);
		ImGui::SliderFloat("Moon Size", &m_fSunSizeNight, 0.990f, 0.999f);
	}
	else
	{
		ImGui::SliderFloat("Sun Size", &m_fSunSizeDay, 0.990f, 0.999f);
	}

	ImGui::SliderFloat("Sun Core Intense", &m_fSunCoreIntensity, 0.1f, 3.0f);

	// A button to return to AUTO Original Color Mode
	if (ImGui::Button("Reset to Auto"))
	{
		m_bManualOverride = false;
		m_fStarDensity = m_fStarBrightness = 1.0f;
		m_fSunSizeDay = 0.9999f;    // Increased from 0.995 (smaller sun)
		m_fSunSizeNight = 0.9995f;  // Increased from 0.999 (smaller sun)
		m_fSunCoreIntensity = 2.0f;
	}
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
	safe_delete(m_pSkyboxScreenSpace);
}

