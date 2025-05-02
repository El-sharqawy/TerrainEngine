#pragma once

#include <glad/glad.h>

class CSaveViewport
{
public:
	CSaveViewport() = default;

	void Save()
	{
		glGetIntegerv(GL_VIEWPORT, m_iViewport);

	}

	void Restore()
	{
		glViewport(m_iViewport[0], m_iViewport[1], m_iViewport[2], m_iViewport[3]);
	}

private:
	GLint m_iViewport[4];
};

class CFrameBuffer
{
public:
	CFrameBuffer();
	~CFrameBuffer();

	void Init(GLint iWidth, GLint iHeight);

	void InitDSA(GLint iWidth, GLint iHeight);
	void InitNonDSA(GLint iWidth, GLint iHeight);

	void InitSkyBox(GLint iWidth, GLint iHeight);

	void Destroy();

	void Bind();

	void BindForWriting();

	void BindForReading();

	void UnBindWriting();

	void BindTextureForReading(GLenum eTextureUnit);
	void BindDepthForReading(GLenum eTextureUnit);

	void BindToDefaultBuffer();

	void Resize(GLint iNewWidth, GLint iNewHeight);

	GLuint GetTextureID() const;
	GLuint GetDepthTextureID() const;
	GLuint GetFrameBuffer() const;

	GLint GetWidth() const;
	GLint GetHeight() const;

private:
	GLint m_iWidth;
	GLint m_iHeight;
	GLuint m_uiFBO;
	GLuint m_uiTextureBuffer;
	GLuint m_uiDepthBuffer;
	CSaveViewport m_gSaveViewport;
};

class CShadowFrameBuffer
{
public:
	CShadowFrameBuffer();
	~CShadowFrameBuffer();

	bool Initialize(GLint iWidth, GLint iHeight);
	void BindForWriting();
	void BindTextureForReading(GLenum eTextureUnit);

private:
	GLint m_iWidth;
	GLint m_iHeight;
	GLuint m_uiFBO;
	GLuint m_uiShadowMap;
	CSaveViewport m_gSaveViewport;
};