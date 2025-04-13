#include "stdafx.h"
#include "skybox_framebuffer.h"	
#include "../../LibGL/source/window.h"

void BindFrameBuffer(GLuint frameBuffer, GLint width, GLint height)
{
	glBindTexture(GL_TEXTURE_2D, 0); //To make sure the texture isn't bound
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

void UnbindCurrentFrameBuffer(GLint scrWidth, GLint scrHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, scrWidth, scrHeight);
}

void UnbindCurrentFrameBuffer(CWindow* pWindow)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, pWindow->GetWidth(), pWindow->GetHeight());
}

GLuint CreateFrameBuffer()
{
	GLuint uiFBO;
	glGenFramebuffers(1, &uiFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, uiFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);


	return (uiFBO);
}

GLuint CreateTextureAttachment(GLint width, GLint height)
{
	GLuint uiTexture;
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, uiTexture, 0);

	return (uiTexture);
}

GLuint* CreateColorAttachments(GLint width, GLint height, GLuint nColorAttachments)
{
	GLuint* pColorAttachments = new GLuint[nColorAttachments];
	glGenTextures(nColorAttachments, pColorAttachments);

	for (GLuint i = 0; i < nColorAttachments; i++)
	{
		glBindTexture(GL_TEXTURE_2D, pColorAttachments[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, pColorAttachments[i], 0);
	}

	return pColorAttachments;
}

GLuint CreateDepthTextureAttachment(GLint width, GLint height)
{
	GLuint uiTexture;
	glGenTextures(1, &uiTexture);
	glBindTexture(GL_TEXTURE_2D, uiTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, uiTexture, 0);

	return (uiTexture);
}

GLuint CreateDepthBufferAttachment(GLint width, GLint height)
{
	GLuint uiDepthBuff;
	glGenRenderbuffers(1, &uiDepthBuff);
	glBindRenderbuffer(GL_RENDERBUFFER, uiDepthBuff);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, uiDepthBuff);
	return (uiDepthBuff);
}

GLuint CreateRenderBufferAttachment(GLint width, GLint height)
{
	GLuint uiRBO;
	glGenRenderbuffers(1, &uiRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, uiRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, uiRBO); // now Attach it

	return (uiRBO);
}

FrameBufferObject::FrameBufferObject(GLint iWidth, GLint iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_uiFBO = CreateFrameBuffer();
	m_uiTex = CreateTextureAttachment(iWidth, iHeight);
	m_uiDepthTex = CreateDepthTextureAttachment(iWidth, iHeight);

	m_pColorAttachments = nullptr;
	m_iNColorAttachments = 1;
	m_uiRenderBuffer = 0; // Maybe use later?
}

FrameBufferObject::FrameBufferObject(GLint iWidth, GLint iHeight, GLint iNumColorAttachments)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_uiFBO = CreateFrameBuffer();
	m_uiTex = 0;
	m_uiDepthTex = CreateDepthTextureAttachment(iWidth, iHeight);

	m_pColorAttachments = CreateColorAttachments(iWidth, iHeight, iNumColorAttachments);
	m_iNColorAttachments = iNumColorAttachments;
	m_uiRenderBuffer = 0; // Maybe use later?

	GLuint* pColorAttachmentFlags = new GLuint[iNumColorAttachments];
	for (GLint i = 0; i < iNumColorAttachments; i++)
	{
		pColorAttachmentFlags[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(iNumColorAttachments, pColorAttachmentFlags);
	safe_delete_arr(pColorAttachmentFlags);
}

GLuint FrameBufferObject::GetColorAttachmentTex(GLint i)
{
	if (i < 0 || i >= m_iNColorAttachments)
	{
		sys_err("FrameBufferObject::GetColorAttachmentTex: Color Attachment out of range! %d - %d ", i, m_iNColorAttachments);
		return (0);
	}

	return (m_pColorAttachments[i]);
}

void FrameBufferObject::Bind()
{
	BindFrameBuffer(m_uiFBO, m_iWidth, m_iHeight);
}

void FrameBufferObject::UnBind()
{
	BindFrameBuffer(0, m_iWidth, m_iHeight);
}

GLuint FrameBufferObject::GetDepthTexture()
{
	return (m_uiDepthTex);
}

GLuint FrameBufferObject::GetColorTexture()
{
	return (m_uiTex);
}

GLuint FrameBufferObject::GetFrameBuffer()
{
	return (m_uiFBO);
}