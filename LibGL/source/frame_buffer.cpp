#include "stdafx.h"
#include "frame_buffer.h"

CFrameBuffer::CFrameBuffer() : m_gSaveViewport()
{
	m_iWidth = 0;
	m_iHeight = 0;
	m_uiFBO = 0;
	m_uiTextureBuffer = 0;
	m_uiDepthBuffer = 0;
}

CFrameBuffer::~CFrameBuffer()
{
	if (m_uiFBO)
	{
		glDeleteFramebuffers(1, &m_uiFBO);
	}

	if (m_uiTextureBuffer)
	{
		glDeleteTextures(1, &m_uiTextureBuffer);
	}

	if (m_uiDepthBuffer)
	{
		glDeleteTextures(1, &m_uiDepthBuffer);
	}
}

void CFrameBuffer::Init(GLint iWidth, GLint iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	if (IsGLVersionHigher(4, 5))
	{
		InitDSA(iWidth, iHeight);
	}
	else
	{
		InitNonDSA(iWidth, iHeight);
	}
}

void CFrameBuffer::InitSkyBox(GLint iWidth, GLint iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	glGenFramebuffers(1, &m_uiFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFBO);

	glGenTextures(1, &m_uiTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTextureBuffer, 0);

	GLExitIfError();

	glGenTextures(1, &m_uiDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, m_uiDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, iWidth, iHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uiDepthBuffer, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		sys_err("FrameBuffer::InitTest error, status: 0x%x\n", Status);
		exit(0);
	}

	sys_log("CFrameBuffer::InitTest FrameBuffer: %u have been created", m_uiFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void CFrameBuffer::InitDSA(GLint iWidth, GLint iHeight)
{
	sys_log("CFrameBuffer::InitDSA");

	glCreateFramebuffers(1, &m_uiFBO);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_uiTextureBuffer);
	glTextureStorage2D(m_uiTextureBuffer, 1, GL_RGB8, m_iWidth, m_iHeight);
	glTextureParameteri(m_uiTextureBuffer, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_uiTextureBuffer, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_uiTextureBuffer, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_uiTextureBuffer, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLExitIfError();

	glCreateTextures(GL_TEXTURE_2D, 1, &m_uiDepthBuffer);
	glTextureStorage2D(m_uiDepthBuffer, 1, GL_DEPTH_COMPONENT24, m_iWidth, m_iHeight);
	glTextureParameteri(m_uiDepthBuffer, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_uiDepthBuffer, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_uiDepthBuffer, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_uiDepthBuffer, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glNamedFramebufferTexture(m_uiFBO, GL_COLOR_ATTACHMENT0, m_uiTextureBuffer, 0);
	glNamedFramebufferTexture(m_uiFBO, GL_DEPTH_ATTACHMENT, m_uiDepthBuffer, 0);

	glNamedFramebufferDrawBuffer(m_uiFBO, GL_COLOR_ATTACHMENT0);
	glNamedFramebufferReadBuffer(m_uiFBO, GL_NONE);

	GLenum Status = glCheckNamedFramebufferStatus(m_uiFBO, GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		sys_err("FrameBuffer::InitDSA error, status: 0x%x\n", Status);
		exit(0);
	}

	sys_log("CFrameBuffer::InitDSA FrameBuffer: %u have been created", m_uiFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBuffer::InitNonDSA(GLint iWidth, GLint iHeight)
{
	sys_log("CFrameBuffer::InitNonDSA");

	glGenFramebuffers(1, &m_uiFBO);

	glGenTextures(1, &m_uiTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLExitIfError();

	glGenTextures(1, &m_uiDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, m_uiDepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, iWidth, iHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, m_uiFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTextureBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uiDepthBuffer, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		sys_err("FrameBuffer::InitNonDSA error, status: 0x%x\n", Status);
		exit(0);
	}

	sys_log("CFrameBuffer::InitNonDSA FrameBuffer: %u have been created", m_uiFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CFrameBuffer::Bind()
{
	m_gSaveViewport.Save();

	glBindTexture(GL_TEXTURE_2D, 0); //To make sure the texture isn't bound (does not clear the skybox framebuffer)
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiFBO);
	glViewport(0, 0, m_iWidth, m_iHeight);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CFrameBuffer::BindForWriting()
{
	m_gSaveViewport.Save();

	// Bind the framebuffer for drawing using DSA
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_uiFBO);
	glViewport(0, 0, m_iWidth, m_iHeight);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CFrameBuffer::BindToDefaultBuffer()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, GetFrameBuffer());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);           // 0 = default window framebuffer
	glBlitFramebuffer(
		0, 0, m_iWidth, m_iHeight,    // source rect (in your FBO)
		0, 0, m_iWidth, m_iHeight,    // dest rect (in window)
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR     // or GL_LINEAR if you prefer filtering
	);
}

void CFrameBuffer::UnBindWriting()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_gSaveViewport.Restore();
}

void CFrameBuffer::BindTextureForReading(GLenum eTextureUnit)
{
	if (IsGLVersionHigher(4, 5))
	{
		glBindTextureUnit(eTextureUnit - GL_TEXTURE0, m_uiTextureBuffer);
	}
	else
	{
		glActiveTexture(eTextureUnit);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureBuffer);
	}
}

void CFrameBuffer::BindDepthForReading(GLenum eTextureUnit)
{
	if (IsGLVersionHigher(4, 5))
	{
		glBindTextureUnit(eTextureUnit - GL_TEXTURE0, m_uiDepthBuffer);
	}
	else
	{
		glActiveTexture(eTextureUnit);
		glBindTexture(GL_TEXTURE_2D, m_uiDepthBuffer);
	}
}

GLuint CFrameBuffer::GetTextureID() const
{
	return (m_uiTextureBuffer);
}

GLuint CFrameBuffer::GetDepthTextureID() const
{
	return (m_uiDepthBuffer);
}

GLuint CFrameBuffer::GetFrameBuffer() const
{
	return (m_uiFBO);
}

void CFrameBuffer::Resize(GLint iNewWidth, GLint iNewHeight)
{
	m_iWidth = iNewWidth;
	m_iHeight = iNewHeight;

	// Resize color texture
	if (IsGLVersionHigher(4, 5))
	{
		glTextureStorage2D(m_uiTextureBuffer, 1, GL_RGB8, m_iWidth, m_iHeight);
		glTextureStorage2D(m_uiDepthBuffer, 1, GL_DEPTH_COMPONENT24, m_iWidth, m_iHeight);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_uiTextureBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

		glBindTexture(GL_TEXTURE_2D, m_uiDepthBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_iWidth, m_iHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
}
