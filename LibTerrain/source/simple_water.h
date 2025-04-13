#pragma once

#include <memory>

#include "triangle_list.h"
#include "../../LibGL/source/frame_buffer.h"
#include "../../LibGL/source/texture.h"
#include "../../LibGL/source/shader.h"

class CSimpleWater
{
public:
	CSimpleWater();
	~CSimpleWater() = default;

	void Init(GLint iSize, float fWorldScale);
	
	void Render(const SVector3Df& v3CameraPos, const CMatrix4Df& viewProj, const SVector3Df& v3LightDir);

	void StartReflectionPass();
	void StopReflectionPass();

	void StartRefractionPass();
	void StopRefractionPass();

	void SetWaterHeight(const float fHeight);
	float GetWaterHeight() const;

	GLuint GetNormalMapTexture() const;
	GLuint GetDUDVMapTexture() const;

	GLuint GetReflectionTexture() const;
	GLuint GetRefractionTexture() const;

	CWaterTriangleList& GetTriangleList() { return m_gWater; }

private:
	CWaterTriangleList m_gWater;
	std::unique_ptr<CShader> m_pWaterShader;
	float m_fWaterHeight;
	CFrameBuffer m_gReflectionFBO;
	CFrameBuffer m_gRefractionFBO;
	CTexture m_gDudvMapTex;
	CTexture m_gNormalMapTex;
	CCamera* m_pCamera;
};