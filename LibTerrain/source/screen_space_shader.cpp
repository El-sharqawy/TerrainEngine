#include "stdafx.h"
#include "screen_space_shader.h"

bool CScreenSpaceShader::m_bInitialized = false;
GLuint CScreenSpaceShader::m_uiQuadVAO = 0;
GLuint CScreenSpaceShader::m_uiQuadVBO = 0;

CScreenSpaceShader::CScreenSpaceShader(const std::string& stFragmentShader, const std::string& stName)
{
	InitializeQuad();

	m_pScreenShader = new CShader(stName);
	m_pScreenShader->AttachShader("shaders/screen.vert");
	m_pScreenShader->AttachShader(stFragmentShader.c_str());
	m_pScreenShader->LinkPrograms();
}

CScreenSpaceShader::~CScreenSpaceShader()
{
	delete m_pScreenShader;
	m_pScreenShader = nullptr;
}

CShader* const CScreenSpaceShader::GetShaderPtr()
{
	assert(m_pScreenShader);
	return (m_pScreenShader);
}

CShader& CScreenSpaceShader::GetShader() const
{
	assert(m_pScreenShader);
	return (*m_pScreenShader);
}

void CScreenSpaceShader::Render()
{
	DrawQuad();
}

void CScreenSpaceShader::DrawQuad()
{
	glBindVertexArray(m_uiQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void CScreenSpaceShader::EnableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

void CScreenSpaceShader::DisableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
}

void CScreenSpaceShader::InitializeQuad()
{
	if (!m_bInitialized)
	{
		const float vertices[] = {
			// pos       // tex
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
		};

		if (IsGLVersionHigher(4, 5))
		{
			// Create vertex array and buffer
			glCreateVertexArrays(1, &m_uiQuadVAO);
			glCreateBuffers(1, &m_uiQuadVBO);

			// Upload vertex data to the buffer
			glNamedBufferStorage(m_uiQuadVBO, sizeof(vertices), vertices, 0);

			// Associate buffer with the VAO at binding index 0
			glVertexArrayVertexBuffer(m_uiQuadVAO, 0, m_uiQuadVBO, 0, 4 * sizeof(float));

			// Set up vertex attribute 0 (vec2 position)
			glEnableVertexArrayAttrib(m_uiQuadVAO, 0);
			glVertexArrayAttribFormat(m_uiQuadVAO, 0, 2, GL_FLOAT, GL_FALSE, 0);					// offset = 0
			glVertexArrayAttribBinding(m_uiQuadVAO, 0, 0);											// bind to binding 0

			// Set up vertex attribute 1 (vec2 texcoord)
			glEnableVertexArrayAttrib(m_uiQuadVAO, 1);
			glVertexArrayAttribFormat(m_uiQuadVAO, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));	// offset = 2 floats
			glVertexArrayAttribBinding(m_uiQuadVAO, 1, 0);											// bind to binding 1
			m_bInitialized = true;
		}
		else
		{
			// Create vertex array and buffer
			glGenVertexArrays(1, &m_uiQuadVAO);
			glBindVertexArray(m_uiQuadVAO);

			glGenBuffers(1, &m_uiQuadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_uiQuadVBO);

			// Upload vertex data to the buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			// Set up vertex attribute 0 (vec2 position)
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)0);						// offset = 0
			glEnableVertexAttribArray(0);																			// bind to binding 0

			// Set up vertex attribute 1 (vec2 texcoord)
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*) (2 * sizeof(float)));	// offset = 2 floats
			glEnableVertexAttribArray(1);																			// bind to binding 1
			CScreenSpaceShader::m_bInitialized = true;
		}
	}
}