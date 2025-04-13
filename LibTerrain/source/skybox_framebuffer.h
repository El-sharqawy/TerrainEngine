#pragma once

#include <glad/glad.h>

class CWindow;

void BindFrameBuffer(GLuint frameBuffer, GLint width, GLint height);
void UnbindCurrentFrameBuffer(GLint scrWidth, GLint scrHeight);
void UnbindCurrentFrameBuffer(CWindow* pWindow);
GLuint CreateFrameBuffer();
GLuint CreateTextureAttachment(GLint width, GLint height);
GLuint* CreateColorAttachments(GLint width, GLint height, GLuint nColorAttachments);
GLuint CreateDepthTextureAttachment(GLint width, GLint height);
GLuint CreateDepthBufferAttachment(GLint width, GLint height);
GLuint CreateRenderBufferAttachment(GLint width, GLint height);

class FrameBufferObject
{
public:
	FrameBufferObject(GLint iWidth, GLint iHeight);
	FrameBufferObject(GLint iWidth, GLint iHeight, GLint iNumColorAttachments);
	GLuint GetColorAttachmentTex(GLint i);
	void Bind();
	void UnBind();

	GLuint GetDepthTexture();
	GLuint GetColorTexture();
	GLuint GetFrameBuffer();

private:
	GLuint m_uiFBO, m_uiRenderBuffer, m_uiDepthTex;
	GLuint m_uiTex;
	GLint m_iWidth, m_iHeight;
	GLint m_iNColorAttachments;
	GLuint* m_pColorAttachments;
};
