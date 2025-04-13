#pragma once

#include "object.h"
#include "../../LibGL/source/shader.h"

class CScreenSpaceShader : public CObject
{
public:
	CScreenSpaceShader(const std::string& stFragmentShader, const std::string& stName);
	~CScreenSpaceShader();

	CShader* const GetShaderPtr();
	CShader& GetShader() const;

	virtual void Render();

	static void DrawQuad();
	static void EnableDepthTest();
	static void DisableDepthTest();

protected:
	void InitializeQuad();

private:
	static bool m_bInitialized;
	static GLuint m_uiQuadVAO, m_uiQuadVBO;
	CShader* m_pScreenShader;
};
