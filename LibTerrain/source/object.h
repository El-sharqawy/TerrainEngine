#pragma once

#include "../../LibMath/source/stdafx.h"
#include "skybox_framebuffer.h"

struct SSceneElements
{
	SVector3Df v3LightPos, v3LightColor, v3LightDir, v3FogColor, v3Seed;
	CMatrix4Df mProjMatrix;
	CCamera* pCamera;
	FrameBufferObject* pSceneFBO;
	bool bWireFrame = false;
};

class CObject
{
public:
	virtual void Render() = 0;
	virtual void SetGUI() {}

	//if the class will cointain some logic, so it must be refreshed at each game loop cycle by calling update. Otherwise just don't override it.  
	virtual void Update() {}

	inline static SSceneElements* pScene;
};