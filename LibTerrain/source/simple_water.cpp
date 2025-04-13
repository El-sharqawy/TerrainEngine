#include "stdafx.h"
#include "simple_water.h"

CSimpleWater::CSimpleWater() : m_gDudvMapTex(GL_TEXTURE_2D), m_gNormalMapTex(GL_TEXTURE_2D), m_gReflectionFBO(), m_gRefractionFBO()
{
	m_pWaterShader = std::make_unique<CShader>("WaterShader");
	m_pWaterShader->AttachShader("shaders/simple_water.vert");
	m_pWaterShader->AttachShader("shaders/simple_water.frag");
	m_pWaterShader->LinkPrograms();

	m_fWaterHeight = 100.0f;
	m_pCamera = CCameraManager::Instance().GetCurrentCamera();
}

void CSimpleWater::Init(GLint iSize, float fWorldScale)
{
	m_pWaterShader->Use();
	m_pWaterShader->setInt("ReflectionTexture", REFLECTION_TEXTURE_UNIT_INDEX);
	m_pWaterShader->setInt("RefractionTexture", REFRACTION_TEXTURE_UNIT_INDEX);
	m_pWaterShader->setInt("DUDVMapTexture", DUDV_TEXTURE_UNIT_INDEX);
	m_pWaterShader->setInt("NormalMapTexture", NORMAL_MAP_TEXTURE_UNIT_INDEX);
	m_pWaterShader->setInt("DepthMapTexture", DEPTH_MAP_TEXTURE_UNIT_INDEX);
	m_pWaterShader->setFloat("fWaterHeight", m_fWaterHeight);
	m_pWaterShader->setVec3("v3LightColor", SVector3Df(1.0f, 1.0f, 1.0f));

	m_gDudvMapTex.Load("resources/textures/waterDUDV.png");
	m_gNormalMapTex.Load("resources/textures/WaterNormalMap.png");
	
	m_gWater.CreateTriangleList(iSize, iSize, fWorldScale);
	m_gReflectionFBO.Init(1000, 1000);
	m_gRefractionFBO.Init(1000, 1000);
}

void CSimpleWater::Render(const SVector3Df& v3CameraPos, const CMatrix4Df& viewProj, const SVector3Df& v3LightDir)
{
	m_pWaterShader->Use();
	m_pWaterShader->setMat4("ViewMatrix", viewProj);
	m_pWaterShader->setVec3("v3CameraPos", v3CameraPos);

	SVector3Df ReversedLightDir = v3LightDir * -1.0f;
	ReversedLightDir.normalize();
	m_pWaterShader->setVec3("v3ReservedLightDir", ReversedLightDir);

	m_pWaterShader->setFloat("fWaterHeight", m_fWaterHeight);

	static float sTime = 0.0f;
	sTime += 0.001f;
	m_pWaterShader->setFloat("fDUDVMapTextureOffset", sTime * 0.05f);

	float time = static_cast<float>(glfwGetTime());
	m_pWaterShader->setFloat("fTime", time);

	m_gReflectionFBO.BindTextureForReading(REFLECTION_TEXTURE_UNIT);
	m_gRefractionFBO.BindTextureForReading(REFRACTION_TEXTURE_UNIT);
	m_gRefractionFBO.BindDepthForReading(DEPTH_MAP_TEXTURE_UNIT);

	m_gDudvMapTex.Bind(DUDV_TEXTURE_UNIT);
	m_gNormalMapTex.Bind(NORMAL_MAP_TEXTURE_UNIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_gWater.Render();
	glDisable(GL_BLEND);
}

void CSimpleWater::StartReflectionPass()
{
	m_gReflectionFBO.BindForWriting();
}

void CSimpleWater::StopReflectionPass()
{
	m_gReflectionFBO.UnBindWriting();
}

void CSimpleWater::StartRefractionPass()
{
	m_gRefractionFBO.BindForWriting();
}

void CSimpleWater::StopRefractionPass()
{
	m_gRefractionFBO.UnBindWriting();
}

void CSimpleWater::SetWaterHeight(const float fHeight)
{
	m_fWaterHeight = fHeight;
}

float CSimpleWater::GetWaterHeight() const
{
	return (m_fWaterHeight);
}

GLuint CSimpleWater::GetNormalMapTexture() const
{
	return (m_gNormalMapTex.GetTextureID());
}

GLuint CSimpleWater::GetDUDVMapTexture() const
{
	return (m_gDudvMapTex.GetTextureID());
}

GLuint CSimpleWater::GetReflectionTexture() const
{
	return (m_gReflectionFBO.GetTextureID());
}

GLuint CSimpleWater::GetRefractionTexture() const
{
	return (m_gRefractionFBO.GetTextureID());
}