#pragma once

#include "../../LibGL/source/shader.h"
#include "skybox.h"
#include "object.h"
#include "screen_space_shader.h"

// CloudsObject is responsible to collect the attributes and shaders that will be needed
// to render the volumetric clouds. Also, it creates the noises which models the clouds.
class CCloudsObject : public CObject
{
public:
	CCloudsObject(TSceneElements* pScene, CSkyBox* pSky);
	~CCloudsObject();

	// fake implementation, it's needed to let this class being a drawableObject to subscribe to GUI class. must be fixed
	virtual void Render() {};

	virtual void Update();
	virtual void SetGUI();

protected:
	void InitializeVariables();
	void InitializeShaders();

	void GenerateModelTextures();
	void GenerateWeatherMap();

private:
	CShader* m_pVolumetricCloudsShader;
	CShader* m_pWeatherShader;

	CScreenSpaceShader* m_pPostProcessingShader;
	TSceneElements* m_pScene;
	CSkyBox* m_pSkyBox;

	// Variables
	GLfloat m_fCoverage, m_fCloudSpeed, m_fCrispiness, m_fCurliness, m_fDensity, m_fAbsorption;
	GLfloat m_fEarthRadius, m_fSphereInnerRadius, m_fSphereOuterRadius;
	GLfloat m_fPerlinFrequency;

	GLboolean m_bEnableRays, m_bEnablePowder, m_bPostProcess;

	SVector3Df m_v3CloudColorTop, m_v3CloudColorBottom;
	SVector3Df m_v3Seed, m_v3OldSeed;

	CTexture* m_pPerlinTex;
	CTexture* m_pWorley32Tex;
	CTexture* m_pWeatherTex;
};