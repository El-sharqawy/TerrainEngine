#pragma once

#include "../../LibGL/source/shader.h"
#include "../../LibGL/source/window.h"
#include "../../LibGL/source/frame_buffer.h"
#include "object.h"
#include "screen_space_shader.h"
#include "skybox_framebuffer.h"

typedef struct SColorPreset
{
	SVector3Df v3CloudColorBottom, v3SkyColorTop, v3SkyColorBottom;
	SVector3Df v3LightColor, v3FogColor;
} TColorPreset;

class CSkyBox : public CSingleton<CSkyBox>, public CObject
{
public:
	CSkyBox(CWindow* pWindow);
	~CSkyBox();

	virtual void Render();
	virtual void SetGUI();

	//if the class will cointain some logic, so it must be refreshed at each game loop cycle by calling update. Otherwise just don't override it.  
	virtual void Update();

	TColorPreset DefaultPreset();
	TColorPreset SunsetPreset();
	TColorPreset SunsetPresetTwo();

	void MixSkyColorPresets(GLfloat fVal, const TColorPreset& p1, const TColorPreset& p2);

	SVector3Df GetSkyColorTop() { return m_v3SkyColorTop; }
	SVector3Df GetSkyColorBottom() { return m_v3SkyColorBottom; }

	CFrameBuffer* GetFBO() { return m_pSkyBoxNew; }

private:
	SVector3Df m_v3SkyColorTop, m_v3SkyColorBottom;
	CScreenSpaceShader* m_pSkyboxScreenSpace;
	CFrameBuffer* m_pSkyBoxNew;
	TColorPreset m_sPresetSunset, m_sHighSunPreset;
	CWindow* m_pWindow;

	bool m_bManualOverride;

	// Night Uniformms
	bool m_bIsNight;
	float m_fStarDensity, m_fStarBrightness;

	// Sun Uniforms
	float m_fSunSizeDay, m_fSunSizeNight, m_fSunCoreIntensity;
};