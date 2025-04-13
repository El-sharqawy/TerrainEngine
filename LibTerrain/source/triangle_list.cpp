#include "stdafx.h"
#include "triangle_list.h"
#include "terrain.h"

CTriangleList::CTriangleList()
{
	m_iWidth = 0;
	m_iDepth = 0;
	m_iVAO = 0;
	m_iVBO = 0;
	m_iIdxBuf = 0;
}

CTriangleList::~CTriangleList()
{
	Destroy();
}

void CTriangleList::Destroy()
{
	if (m_iVAO > 0)
	{
		glDeleteVertexArrays(1, &m_iVAO);
	}
	if (m_iVBO > 0)
	{
		glDeleteBuffers(1, &m_iVBO);
	}
	if (m_iIdxBuf > 0)
	{
		glDeleteBuffers(1, &m_iIdxBuf);
	}
}

void CTriangleList::CreateTriangleList(GLint Width, GLint Depth, const CBaseTerrain* pTerrain)
{
	m_iWidth = Width;
	m_iDepth = Depth;

	CreateGLState();
	PopulateBuffers(pTerrain);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	sys_log("CTriangleList::CreateTriangleList Created Triangle List Size: %d, Width: %d, Depth: %d", Width * Depth, Width, Depth);
}

void CTriangleList::CreateGLState()
{
	glGenVertexArrays(1, &m_iVAO);
	glBindVertexArray(m_iVAO);

	glGenBuffers(1, &m_iVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);

	glGenBuffers(1, &m_iIdxBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIdxBuf);

	const GLint POS_LOC = 0;
	const GLint	TEX_LOC = 1;
	const GLint	NORMALS_LOC = 2;

	size_t NumFloats = 0;

	glEnableVertexAttribArray(POS_LOC);
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(TVertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;
	glEnableVertexAttribArray(TEX_LOC);
	glVertexAttribPointer(TEX_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 2;
	glEnableVertexAttribArray(NORMALS_LOC);
	glVertexAttribPointer(NORMALS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(TVertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;
}

void CTriangleList::PopulateBuffers(const CBaseTerrain* pTerrain)
{
	m_vVertices.resize(m_iWidth * m_iDepth);

	InitVertices(pTerrain);
	GLint numQuads = (m_iWidth - 1) * (m_iDepth - 1);
	m_vIndices.resize(numQuads * 6);
	InitIndices();

	CalculateNormals();

	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vVertices[0]) * m_vVertices.size(), &m_vVertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vIndices[0]) * m_vIndices.size(), &m_vIndices[0], GL_STATIC_DRAW);
}

void CTriangleList::TVertex::InitVertex(const CBaseTerrain* pTerrain, GLint x, GLint z)
{
	const float y = pTerrain->GetHeight(x, z);
	const float WorldScale = pTerrain->GetWorldScale();
	v3Pos = SVector3Df(x * WorldScale, y, z * WorldScale);

	const float fSize = static_cast<float>(pTerrain->GetSize());
	const float TextureScale = pTerrain->GetTextureScale();
	const float fXCoord = (TextureScale * x) / fSize;
	const float fzCoord = (TextureScale * z) / fSize;
	v2TexCoords = SVector2Df(fXCoord, fzCoord);
}

void CTriangleList::InitVertices(const CBaseTerrain* pTerrain)
{
	GLint Index = 0;

	for (GLint z = 0; z < m_iDepth; z++)
	{
		for (GLint x = 0; x < m_iWidth; x++)
		{
			assert(Index < m_vVertices.size());
			m_vVertices[Index].InitVertex(pTerrain, x, z);
			Index++;
		}
	}

	assert(Index == m_vVertices.size());
}

void CTriangleList::InitIndices()
{
	GLuint Index = 0;
	
	for (GLint z = 0; z < m_iDepth - 1; z++)
	{
		for (GLint x = 0; x < m_iWidth - 1; x++)
		{
			GLuint IndexBottomLeft = z * m_iWidth + x;
			GLuint IndexTopLeft = (z + 1) * m_iWidth + x;
			GLuint IndexTopRight = (z + 1) * m_iWidth + x + 1;
			GLuint IndexBottomRight = z * m_iWidth + x + 1;

			// Add Top Left Triangle
			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexBottomLeft;

			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexTopLeft;

			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexTopRight;

			// Add Bottom Right Triangle
			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexBottomLeft;

			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexTopRight;

			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexBottomRight;
		}
	}

	assert(Index == m_vIndices.size());
}

void CTriangleList::CalculateNormals()
{
	GLuint dwIndex = 0;

	// Accumulate each triangle normal into each of triangle vertices

	for (GLuint i = 0; i < m_vIndices.size(); i += 3)
	{
		GLuint uiIndex0 = m_vIndices[i];
		GLuint uiIndex1 = m_vIndices[i + 1];
		GLuint uiIndex2 = m_vIndices[i + 2];

		SVector3Df v1 = m_vVertices[uiIndex1].v3Pos - m_vVertices[uiIndex0].v3Pos;
		SVector3Df v2 = m_vVertices[uiIndex2].v3Pos - m_vVertices[uiIndex0].v3Pos;

		SVector3Df v3Normals = v1.cross(v2);

		v3Normals.normalize();

		m_vVertices[uiIndex0].v3Normals += v3Normals;
		m_vVertices[uiIndex1].v3Normals += v3Normals;
		m_vVertices[uiIndex2].v3Normals += v3Normals;
	}

	// Normalize all the vertex normals
	for (GLuint i = 0; i < m_vVertices.size(); i++)
	{
		m_vVertices[i].v3Normals.normalize();
	}
}

void CTriangleList::Render()
{
	if (m_iVAO == 0 || m_iVBO == 0 || m_iIdxBuf == 0)
	{
		printf("Failed to create VAO, VBO, or Index Buffer.\n");
		exit(1);
	}

	glBindVertexArray(m_iVAO);
	glDrawElements(GL_TRIANGLES, (m_iDepth - 1) * (m_iWidth - 1) * 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

CWaterTriangleList::CWaterTriangleList()
{
	m_iWidth = 0;
	m_iDepth = 0;
	m_iVAO = 0;
	m_iVBO = 0;
	m_iIdxBuf = 0;
}

CWaterTriangleList::~CWaterTriangleList()
{
	Destroy();
}

void CWaterTriangleList::Destroy()
{
	if (m_iVAO > 0)
	{
		glDeleteVertexArrays(1, &m_iVAO);
	}
	if (m_iVBO > 0)
	{
		glDeleteBuffers(1, &m_iVBO);
	}
	if (m_iIdxBuf > 0)
	{
		glDeleteBuffers(1, &m_iIdxBuf);
	}
}

void CWaterTriangleList::SVertex::InitVertex(GLint width, GLint depth, GLint x, GLint z, float fWorldScale)
{
	v3Pos = SVector3Df(x * fWorldScale, 0.0f, z * fWorldScale);

	const float fXCoord = static_cast<float>(x) / static_cast<float>(width);
	const float fzCoord = static_cast<float>(z) / static_cast<float>(depth);
	v2TexCoords = SVector2Df(fXCoord, fzCoord);
}

void CWaterTriangleList::CreateTriangleList(GLint Width, GLint Depth, float fWorldScale)
{
	m_iWidth = Width;
	m_iDepth = Depth;

	CreateGLState();
	PopulateBuffers(fWorldScale);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	sys_log("CWaterTriangleList::CreateTriangleList Created Triangle List Size: %d, Width: %d, Depth: %d", Width * Depth, Width, Depth);

}

void CWaterTriangleList::CreateGLState()
{
	glGenVertexArrays(1, &m_iVAO);
	glBindVertexArray(m_iVAO);

	glGenBuffers(1, &m_iVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);

	glGenBuffers(1, &m_iIdxBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iIdxBuf);

	const GLint POS_LOC = 0;
	const GLint	TEX_LOC = 1;
	const GLint	NORMALS_LOC = 2;

	size_t NumFloats = 0;

	glEnableVertexAttribArray(POS_LOC);
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 3;
	glEnableVertexAttribArray(TEX_LOC);
	glVertexAttribPointer(TEX_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (const void*)(NumFloats * sizeof(float)));
	NumFloats += 2;
	//glEnableVertexAttribArray(NORMALS_LOC);
	//glVertexAttribPointer(NORMALS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (const void*)(NumFloats * sizeof(float)));
	//NumFloats += 3;
}

void CWaterTriangleList::PopulateBuffers(float fWorldScale)
{
	m_vVertices.resize(m_iWidth * m_iDepth);

	sys_log("CWaterTriangleList::PopulateBuffers Created %zu Vertices", m_vVertices.size());

	InitVertices(fWorldScale);

	GLint numQuads = (m_iWidth - 1) * (m_iDepth - 1);
	m_vIndices.resize(numQuads * 6);
	InitIndices();

	CalculateNormals();

	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vVertices[0]) * m_vVertices.size(), &m_vVertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vIndices[0]) * m_vIndices.size(), &m_vIndices[0], GL_STATIC_DRAW);
}

void CWaterTriangleList::CalculateNormals()
{
	GLuint dwIndex = 0;

	// Accumulate each triangle normal into each of triangle vertices

	for (GLuint i = 0; i < m_vIndices.size(); i += 3)
	{
		GLuint uiIndex0 = m_vIndices[i];
		GLuint uiIndex1 = m_vIndices[i + 1];
		GLuint uiIndex2 = m_vIndices[i + 2];

		SVector3Df v1 = m_vVertices[uiIndex1].v3Pos - m_vVertices[uiIndex0].v3Pos;
		SVector3Df v2 = m_vVertices[uiIndex2].v3Pos - m_vVertices[uiIndex0].v3Pos;

		SVector3Df v3Normals = v1.cross(v2);

		v3Normals.normalize();

		m_vVertices[uiIndex0].v3Normals += v3Normals;
		m_vVertices[uiIndex1].v3Normals += v3Normals;
		m_vVertices[uiIndex2].v3Normals += v3Normals;
	}

	// Normalize all the vertex normals
	for (GLuint i = 0; i < m_vVertices.size(); i++)
	{
		m_vVertices[i].v3Normals.normalize();
	}
}

void CWaterTriangleList::InitVertices(float fWorldScale)
{
	GLint Index = 0;
	SVertex vertex;

	for (GLint z = 0; z < m_iDepth; z++)
	{
		for (GLint x = 0; x < m_iWidth; x++)
		{
			vertex.InitVertex(m_iWidth, m_iDepth, x, z, fWorldScale);
			assert(Index < m_vVertices.size());
			m_vVertices[Index++] = vertex;
		}
	}

	assert(Index == m_vVertices.size());
}

void CWaterTriangleList::InitIndices()
{
	GLuint Index = 0;

	for (GLint z = 0; z < m_iDepth - 1; z++)
	{
		for (GLint x = 0; x < m_iWidth - 1; x++)
		{
			GLuint IndexBottomLeft = z * m_iWidth + x;
			GLuint IndexTopLeft = (z + 1) * m_iWidth + x;
			GLuint IndexTopRight = (z + 1) * m_iWidth + x + 1;
			GLuint IndexBottomRight = z * m_iWidth + x + 1;

			// Add Top Left Triangle
			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexBottomLeft;

			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexTopLeft;

			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexTopRight;

			// Add Bottom Right Triangle
			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexBottomLeft;

			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexTopRight;

			assert(Index < m_vIndices.size());
			m_vIndices[Index++] = IndexBottomRight;
		}
	}

	assert(Index == m_vIndices.size());
}

void CWaterTriangleList::Render()
{
	if (m_iVAO == 0 || m_iVBO == 0 || m_iIdxBuf == 0)
	{
		printf("Failed to create VAO, VBO, or Index Buffer.\n");
		exit(1);
	}

	glBindVertexArray(m_iVAO);
	glDrawElements(GL_TRIANGLES, (m_iDepth - 1) * (m_iWidth - 1) * 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void CWaterTriangleList::CircleBrush(GLint iX, GLint iZ, GLfloat fSize, GLfloat fRadius, GLfloat fNewHeight)
{
	sys_log("CWaterTriangleList::CircleBrush: iX: %d, iZ: %d", iX, iZ);

	for (GLint z = 0; z < m_iDepth; z++)
	{
		for (GLint x = 0; x < m_iWidth; x++)
		{
            // Calculate the distance from the center (iX, iZ) to (x, z)
            GLfloat fDist = glm::distance(glm::vec2(iX, iZ), glm::vec2(x, z));

			// If the vertex is within the radius + size buffer, modify its height
			if (fDist <= fRadius + fSize)
			{
				GLint Index = z * m_iWidth + x;
				assert(Index < m_vVertices.size());

				// Compute smooth transition using linear interpolation
				GLfloat blendFactor = glm::clamp((fRadius + fSize - fDist) / fSize, 0.0f, 1.0f);
				m_vVertices[Index].v3Pos.y = glm::mix(m_vVertices[Index].v3Pos.y, fNewHeight, blendFactor);
			}
		}
	}

	UpdateVertexBuffer();
}

void CWaterTriangleList::UpdateVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);  // Bind the vertex buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vVertices[0]) * m_vVertices.size(), m_vVertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind after update
}

