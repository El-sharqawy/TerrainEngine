#pragma once

#include "../../LibMath/source/stdafx.h"
#include "../../LibGL/source/frame_buffer.h"

typedef struct SSceneElements
{
	SVector3Df v3LightPos, v3LightColor, v3LightDir, v3FogColor, v3Seed;
	CMatrix4Df mProjMatrix;
	CCamera* pCamera;
	CFrameBuffer* pSceneFBO;
	bool bWireFrame = false;
	bool bRenderChar = false;
} TSceneElements;

class CObject
{
public:
	virtual void Render() = 0;
	virtual void SetGUI() {}

	//if the class will cointain some logic, so it must be refreshed at each game loop cycle by calling update. Otherwise just don't override it.  
	virtual void Update() {}

	bool bRender = false;

	inline static SSceneElements* pScene;
};