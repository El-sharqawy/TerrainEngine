#include "stdafx.h"
#include "clouds_object.h"

CCloudsObject::CCloudsObject(TSceneElements* pScene, CSkyBox* pSky)
{
	m_pScene = pScene;
	m_pSkyBox = pSky;

	InitializeVariables();
	InitializeShaders();
	GenerateModelTextures();
}

void CCloudsObject::InitializeVariables()
{
	m_fCoverage = 0.45f;
	m_fCloudSpeed = 450.0f;
	m_fCrispiness = 40.0f;
	m_fCurliness = 0.1f;
	m_fDensity = 0.02f;
	m_fAbsorption = 0.35f;

	m_fEarthRadius = 600000.0f;
	m_fSphereInnerRadius = 5000.0f;
	m_fSphereOuterRadius = 17000.0f;

	m_fPerlinFrequency = 0.8f;

	m_bEnableRays = false;
	m_bEnablePowder = false;
	m_bPostProcess = true;

	m_v3CloudColorTop = SVector3Df(169.0f, 149.0f, 149.0f) * (1.5f / 255.0f);
	m_v3CloudColorBottom = SVector3Df(65.0f, 70.0f, 80.0f) * (1.5f / 255.0f);

	m_v3Seed = SVector3Df(0.0f, 0.0f, 0.0f);
	m_v3OldSeed = SVector3Df(0.0f, 0.0f, 0.0f);;

	m_pPerlinTex = nullptr;
	m_pWorley32Tex = nullptr;
	m_pWeatherTex = nullptr;
}

void CCloudsObject::InitializeShaders()
{
	m_pVolumetricCloudsShader = new CShader("VolumetricCloudsShader", "shaders/clouds/volumetric_clouds.comp");
	m_pPostProcessingShader = new CScreenSpaceShader("shaders/clouds/clouds_post.frag", "CloudsPostProcessing Shader");

	m_pWeatherShader = new CShader("WeatherMap");
	m_pWeatherShader->AttachShader("shaders/clouds/weather.comp");
	m_pWeatherShader->LinkPrograms();
}

void CCloudsObject::GenerateModelTextures()
{
	/////////////////// TEXTURE GENERATION //////////////////
	if (!m_pPerlinTex)
	{
		// Make Compute shader
		CShader PerlinWorleyComp("PerlinWorley");
		PerlinWorleyComp.AttachShader("shaders/clouds/perlin_worley.comp");
		PerlinWorleyComp.LinkPrograms();
		
		// Make Texture
		m_pPerlinTex = new CTexture(GL_TEXTURE_3D);
		m_pPerlinTex->GenerateTexture3D(128, 128, 128);

		// Compute
		PerlinWorleyComp.Use();
		PerlinWorleyComp.setVec3("u_resolution", SVector3Df(128.0f, 128.0f, 128.0f));
		PerlinWorleyComp.setInt("outVolTex", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, m_pPerlinTex->GetTextureID());
		glBindImageTexture(0, m_pPerlinTex->GetTextureID(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);

		sys_log("CCloudsObject::GenerateModelTextures: Computing PerlinWorley..");
		glDispatchCompute(MyMath::iceil(128, 4), MyMath::iceil(128, 4), MyMath::iceil(128, 4));
		sys_log("CCloudsObject::GenerateModelTextures: Computed PerlinWorley Successfully!");
		glGenerateMipmap(GL_TEXTURE_3D);
	}

	if (!m_pWorley32Tex)
	{
		// Make Compute shader
		CShader WorleyComp("WorleyComp");
		WorleyComp.AttachShader("shaders/clouds/worley.comp");
		WorleyComp.LinkPrograms();

		// Make Texture
		m_pWorley32Tex = new CTexture(GL_TEXTURE_3D);
		m_pWorley32Tex->GenerateTexture3D(32, 32, 32);

		// Compute
		WorleyComp.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, m_pWorley32Tex->GetTextureID());
		glBindImageTexture(0, m_pWorley32Tex->GetTextureID(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA8);

		sys_log("CCloudsObject::GenerateModelTextures: Computing Worley32..");
		glDispatchCompute(MyMath::iceil(32, 4), MyMath::iceil(32, 4), MyMath::iceil(32, 4));
		sys_log("CCloudsObject::GenerateModelTextures: Computed Worley32 Successfully!");
		glGenerateMipmap(GL_TEXTURE_3D);
	}

	if (!m_pWeatherTex)
	{
		// Make Texture
		m_pWeatherTex = new CTexture(GL_TEXTURE_2D);
		m_pWeatherTex->GenerateTexture2D(1024, 1024);

		// Compute
		GenerateWeatherMap();

		m_v3Seed = m_pScene->v3Seed;
		m_v3OldSeed = m_v3Seed;
	}
}

void CCloudsObject::GenerateWeatherMap()
{
	m_pWeatherTex->BindTexture2D(0);

	m_pWeatherShader->Use();
	m_pWeatherShader->setVec3("seed", m_pScene->v3Seed);
	m_pWeatherShader->setFloat("perlinFrequency", m_fPerlinFrequency);

	sys_log("CCloudsObject::GenerateWeatherMap: Computing Weather..");
	glDispatchCompute(MyMath::iceil(1024, 8), MyMath::iceil(1024, 8), 1);
	sys_log("CCloudsObject::GenerateWeatherMap: Weather Computed Successfully!");

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void CCloudsObject::Update()
{
	m_v3Seed = m_pScene->v3Seed;
	if (m_v3Seed != m_v3OldSeed)
	{
		GenerateWeatherMap();
		m_v3Seed = m_v3OldSeed;
	}
}

void CCloudsObject::SetGUI()
{

}

CCloudsObject::~CCloudsObject()
{
	// Shaders
	safe_delete(m_pVolumetricCloudsShader);
	safe_delete(m_pWeatherShader);
	safe_delete(m_pPostProcessingShader);

	// Textures
	safe_delete(m_pPerlinTex);
	safe_delete(m_pWorley32Tex);
	safe_delete(m_pWeatherTex);
}