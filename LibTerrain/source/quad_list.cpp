#include "stdafx.h"
#include "quad_list.h"
#include "terrain.h"

CQuadList::CQuadList()
{
	m_iWidth = 0;
	m_iDepth = 0;

	m_uiVAO = 0;
	m_uiVBO = 0;
	m_uiIB = 0;

	m_vecVertices.clear();
	m_vecIndices.clear();
}

CQuadList::~CQuadList()
{
	Destroy();
}

void CQuadList::Destroy() const
{
	if (m_uiVAO)
	{
		glDeleteVertexArrays(1, &m_uiVAO);
	}

	if (m_uiVBO)
	{
		glDeleteBuffers(1, &m_uiVBO);
	}

	if (m_uiIB)
	{
		glDeleteBuffers(1, &m_uiIB);
	}
}

void CQuadList::CreateQuadList(GLint iWidth, GLint iDepth, const CBaseTerrain* pTerrain)
{
	m_iWidth = iWidth;
	m_iDepth = iDepth;

	CreateGLState();

	PopulateBuffers(pTerrain);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CQuadList::CreateGLState()
{
	if (IsGLVersionHigher(4, 5))
	{
		// Create and bind VAO
		glCreateVertexArrays(1, &m_uiVAO);
		glBindVertexArray(m_uiVAO);

		// Create VBO using DSA (Direct State Access)
		glCreateBuffers(1, &m_uiVBO);
		
		// Create IBO using DSA
		glCreateBuffers(1, &m_uiIB);

		// Set up vertex attributes using DSA
		const GLint iPos = 0;  // Position attribute location
		const GLint iTex = 1;  // Texture coordinate attribute location

		// Enable vertex attributes
		glEnableVertexArrayAttrib(m_uiVAO, iPos);
		glEnableVertexArrayAttrib(m_uiVAO, iTex);

		size_t sNumFlots = 0;

		// Set up vertex attribute formats using DSA
		// Position attribute (location = 0)
		glVertexArrayAttribFormat(m_uiVAO, iPos, 3, GL_FLOAT, GL_FALSE, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iPos, 0);

		sNumFlots += 3;

		// Texture coordinate attribute (location = 1)
		glVertexArrayAttribFormat(m_uiVAO, iTex, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iTex, 0);

    	// Bind the VBO to the VAO's binding point 0
    	glVertexArrayVertexBuffer(m_uiVAO, 0, m_uiVBO, 0, sizeof(TQuadVertex));

    	// Bind the IBO to the VAO
    	glVertexArrayElementBuffer(m_uiVAO, m_uiIB);
		
	}
	else
	{
		glGenVertexArrays(1, &m_uiVAO);
		glBindVertexArray(m_uiVAO);

		glGenBuffers(1, &m_uiVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);

		glGenBuffers(1, &m_uiIB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIB);

		const GLint iPos = 0;
		const GLint iTex = 1;
		
		size_t sNumFlots = 0;

		// Position attribute (location = 0)
		glEnableVertexAttribArray(iPos);
		glVertexAttribPointer(iPos, 3, GL_FLOAT, GL_FALSE, sizeof(TQuadVertex), (const void*)(sNumFlots * sizeof(float)));

		sNumFlots += 3;

		// Texture coordinate attribute (location = 1)
		glEnableVertexAttribArray(iTex);
		glVertexAttribPointer(iTex, 2, GL_FLOAT, GL_FALSE, sizeof(TQuadVertex), (const void*)(sNumFlots * sizeof(float)));
		sNumFlots += 2;
	}
}

void CQuadList::SQuadVertex::InitQuadVertex(const CBaseTerrain* pTerrain, GLint iWidth, GLint iDepth, GLint iX, GLint iZ)
{
	float WorldScale = pTerrain->GetWorldScale();
	m_v3Pos = SVector3Df(iX * WorldScale, 0.0f, iZ * WorldScale);

	float TextureScale = pTerrain->GetTextureScale();
	m_v2TexPos = SVector2Df(TextureScale * (float)iX / (float)iWidth, TextureScale * (float)iZ / (float)iDepth);
}

void CQuadList::PopulateBuffers(const CBaseTerrain* pTerrain)
{
	size_t numVertices = m_iWidth * m_iDepth;
	m_vecVertices.resize(numVertices);

	InitVertices(pTerrain);

	size_t NumQuads = (m_iWidth - 1) * (m_iDepth - 1);
	m_vecIndices.resize(NumQuads * 4);
	InitIndices();

	if (IsGLVersionHigher(4, 5))
	{
		// Upload vertex data using DSA
		glNamedBufferData(m_uiVBO, sizeof(m_vecVertices[0]) * m_vecVertices.size(), m_vecVertices.data(), GL_STATIC_DRAW);

		// Upload index data using DSA
		glNamedBufferData(m_uiIB, sizeof(m_vecIndices[0]) * m_vecIndices.size(), m_vecIndices.data(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vecVertices[0]) * m_vecVertices.size(), &m_vecVertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vecIndices[0]) * m_vecIndices.size(), &m_vecIndices[0], GL_STATIC_DRAW);
	}
}

void CQuadList::InitVertices(const CBaseTerrain* pTerrain)
{
	GLint Index = 0;

	for (GLint z = 0; z < m_iDepth; z++)
	{
		for (GLint x = 0; x < m_iWidth; x++)
		{
			assert(Index < m_vecVertices.size());
			m_vecVertices[Index].InitQuadVertex(pTerrain, m_iWidth, m_iDepth, x, z);
			Index++;
		}
	}

	assert(Index == m_vecVertices.size());
}

void CQuadList::InitIndices()
{
	GLint Index = 0;

	for (GLint z = 0; z < m_iDepth - 1; z++)
	{
		for (GLint x = 0; x < m_iWidth - 1; x++)
		{
			// Add a single quad
			assert(Index < m_vecIndices.size());
			GLuint IndexBottomLeft = z * m_iWidth + x;
			m_vecIndices[Index++] = IndexBottomLeft;

			assert(Index < m_vecIndices.size());
			GLuint IndexBottomRight = z * m_iWidth + x + 1;
			m_vecIndices[Index++] = IndexBottomRight;

			assert(Index < m_vecIndices.size());
			GLuint IndexTopLeft = (z + 1) * m_iWidth + x;
			m_vecIndices[Index++] = IndexTopLeft;

			assert(Index < m_vecIndices.size());
			GLuint IndexTopRight = (z + 1) * m_iWidth + x + 1;
			m_vecIndices[Index++] = IndexTopRight;
		}
	}

	assert(Index == m_vecIndices.size());
}

void CQuadList::Render() const
{
	glBindVertexArray(m_uiVAO);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawElements(GL_PATCHES, (m_iDepth - 1) * (m_iWidth - 1) * 4, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}

void CQuadList::CircleBrush(EBrushType eBrushType, GLint iX, GLint iZ, GLfloat fSize, GLfloat fRadius, GLfloat fStrength)
{
	if (eBrushType == BRUSH_TYPE_UP)
	{
		// Use only the radius for the circle boundary
		GLfloat influenceRadius = fRadius;
		
		// Calculate the bounds of the area we need to modify
		GLint startX = std::max(0, iX - static_cast<GLint>(influenceRadius));
		GLint endX = std::min(m_iWidth - 1, iX + static_cast<GLint>(influenceRadius));
		GLint startZ = std::max(0, iZ - static_cast<GLint>(influenceRadius));
		GLint endZ = std::min(m_iDepth - 1, iZ + static_cast<GLint>(influenceRadius));

		for (GLint z = startZ; z <= endZ; z++)
		{
			for (GLint x = startX; x <= endX; x++)
			{
				// Calculate the distance from the center (iX, iZ) to (x, z)
				GLfloat fDist = glm::distance(glm::vec2(iX, iZ), glm::vec2(x, z));

				// Only modify vertices within the exact radius
				if (fDist <= influenceRadius)
				{
					GLint Index = z * m_iWidth + x;
					assert(Index < m_vecVertices.size());

					// Calculate normalized distance [0, 1]
					GLfloat normalizedDist = fDist / influenceRadius;

					// Create a smooth, rounded shape
					GLfloat falloff = 0.0f;
					
					// Quadratic falloff for the base
					GLfloat quadraticFalloff = 1.0f - (normalizedDist * normalizedDist);
					
					// Cosine falloff for the top
					GLfloat cosineFalloff = 0.5f * (1.0f + std::cos(normalizedDist * glm::pi<float>()));
					
					// Combine the falloffs
					falloff = 0.7f * quadraticFalloff + 0.3f * cosineFalloff;

					// Apply height change with smooth falloff
					GLfloat heightChange = fStrength * falloff;

					// Add a slight random variation
					GLfloat randomVariation = 0.05f * (std::sin(x * 0.1f) + std::cos(z * 0.1f));
					heightChange += randomVariation * fStrength * falloff;

					m_vecVertices[Index].m_v3Pos.y += heightChange;
				}
			}
		}
	}
	else if (eBrushType == BRUSH_TYPE_DOWN)
	{
		// Use only the radius for the circle boundary
		GLfloat influenceRadius = fRadius;

		// Calculate the bounds of the area we need to modify
		GLint startX = std::max(0, iX - static_cast<GLint>(influenceRadius));
		GLint endX = std::min(m_iWidth - 1, iX + static_cast<GLint>(influenceRadius));
		GLint startZ = std::max(0, iZ - static_cast<GLint>(influenceRadius));
		GLint endZ = std::min(m_iDepth - 1, iZ + static_cast<GLint>(influenceRadius));

		for (GLint z = startZ; z <= endZ; z++)
		{
			for (GLint x = startX; x <= endX; x++)
			{
				// Calculate the distance from the center (iX, iZ) to (x, z)
				GLfloat fDist = glm::distance(glm::vec2(iX, iZ), glm::vec2(x, z));

				// Only modify vertices within the exact radius
				if (fDist <= influenceRadius)
				{
					GLint Index = z * m_iWidth + x;
					assert(Index < m_vecVertices.size());

					// Calculate normalized distance [0, 1]
					GLfloat normalizedDist = fDist / influenceRadius;

					// Create a smooth, rounded shape
					GLfloat falloff = 0.0f;

					// Quadratic falloff for the base
					GLfloat quadraticFalloff = 1.0f - (normalizedDist * normalizedDist);

					// Cosine falloff for the top
					GLfloat cosineFalloff = 0.5f * (1.0f + std::cos(normalizedDist * glm::pi<float>()));

					// Combine the falloffs
					falloff = 0.7f * quadraticFalloff + 0.3f * cosineFalloff;

					// Apply height change with smooth falloff
					GLfloat heightChange = fStrength * falloff;

					// Add a slight random variation
					GLfloat randomVariation = 0.05f * (std::sin(x * 0.1f) + std::cos(z * 0.1f));
					heightChange += randomVariation * fStrength * falloff;

					m_vecVertices[Index].m_v3Pos.y -= heightChange;
				}
			}
		}
	}
	else if (eBrushType == BRUSH_TYPE_SMOOTH)
	{
		// Calculate the smoothing area
		GLint startX = std::max(1, iX - static_cast<GLint>(fSize));
		GLint endX = std::min(m_iWidth - 2, iX + static_cast<GLint>(fSize));
		GLint startZ = std::max(1, iZ - static_cast<GLint>(fSize));
		GLint endZ = std::min(m_iDepth - 2, iZ + static_cast<GLint>(fSize));

		// Create a temporary copy of the heights
		std::vector<GLfloat> tempHeights(m_vecVertices.size());
		for (size_t i = 0; i < m_vecVertices.size(); ++i)
		{
			tempHeights[i] = m_vecVertices[i].m_v3Pos.y;
		}

		// Apply smoothing
		for (GLint z = startZ; z <= endZ; z++)
		{
			for (GLint x = startX; x <= endX; x++)
			{
				GLint Index = z * m_iWidth + x;

				// Average with surrounding vertices
				GLfloat sum = 0.0f;
				GLint count = 0;

				for (GLint dz = -1; dz <= 1; dz++)
				{
					for (GLint dx = -1; dx <= 1; dx++)
					{
						GLint nx = x + dx;
						GLint nz = z + dz;

						if (nx >= 0 && nx < m_iWidth && nz >= 0 && nz < m_iDepth)
						{
							GLint nIndex = nz * m_iWidth + nx;
							sum += tempHeights[nIndex];
							count++;
						}
					}
				}

				// Calculate the smoothed height
				GLfloat smoothedHeight = sum / count;

				// Blend between original and smoothed height based on smooth strength
				// fSmoothStrength should be between 0.0 and 1.0
				// 0.0 = no smoothing, 1.0 = full smoothing
				GLfloat fSmoothFactor = fStrength / 100.0f;
				m_vecVertices[Index].m_v3Pos.y = glm::mix(tempHeights[Index], smoothedHeight, fSmoothFactor);
			}
		}
	}
	else if (eBrushType == BRUSH_TYPE_NOISE)
	{
		// Clamp strength to reasonable range
		fStrength = glm::clamp(fStrength, 0.1f, 10.0f);

		// Use the exact radius without additional size
		GLfloat influenceRadius = fRadius;

		// Calculate the bounds of the area we need to modify
		GLint startX = std::max(0, iX - static_cast<GLint>(influenceRadius));
		GLint endX = std::min(m_iWidth - 1, iX + static_cast<GLint>(influenceRadius));
		GLint startZ = std::max(0, iZ - static_cast<GLint>(influenceRadius));
		GLint endZ = std::min(m_iDepth - 1, iZ + static_cast<GLint>(influenceRadius));

		// Create multiple random number generators for different noise patterns
		std::random_device rd;
		std::mt19937 gen1(rd());
		std::mt19937 gen2(rd());
		std::mt19937 gen3(rd());
		std::uniform_real_distribution<> dis1(-1.0, 1.0);
		std::uniform_real_distribution<> dis2(-1.0, 1.0);
		std::uniform_real_distribution<> dis3(-1.0, 1.0);

		for (GLint z = startZ; z <= endZ; z++)
		{
			for (GLint x = startX; x <= endX; x++)
			{
				GLfloat fDist = glm::distance(glm::vec2(iX, iZ), glm::vec2(x, z));

				// Only modify vertices strictly within the circle
				if (fDist <= influenceRadius)
				{
					GLint Index = z * m_iWidth + x;
					assert(Index < m_vecVertices.size());

					// Calculate a sharp falloff at the circle boundary
					GLfloat normalizedDist = fDist / influenceRadius;
					GLfloat falloff = 1.0f - normalizedDist; // Linear falloff
					falloff = falloff * falloff; // Quadratic falloff for sharper edge

					// Generate multiple layers of noise
					GLfloat noise = 0.0f;

					// Base noise (40% of total noise)
					noise += static_cast<GLfloat>(dis1(gen1)) * fStrength * 0.4f;

					// Medium frequency noise (30% of total noise)
					noise += static_cast<GLfloat>(dis2(gen2)) * fStrength * 0.3f *
						(std::sin(x * 0.2f) + std::cos(z * 0.2f));

					// High frequency noise (30% of total noise)
					noise += static_cast<GLfloat>(dis3(gen3)) * fStrength * 0.3f *
						(std::sin(x * 0.4f) * std::cos(z * 0.4f));

					// Apply the noise with sharp falloff at the boundary
					m_vecVertices[Index].m_v3Pos.y += noise * falloff;

					// Add some position-based variation
					GLfloat positionNoise = std::sin(x * 0.1f) * std::cos(z * 0.1f);
					m_vecVertices[Index].m_v3Pos.y += positionNoise * fStrength * 0.1f * falloff;
				}
			}
		}
	}

	// Update the vertex buffer
	UpdateVertexBuffer();
}

void CQuadList::UpdateVertexBuffer()
{
	if (IsGLVersionHigher(4, 5))
	{
		// Update vertex data using DSA
		glNamedBufferSubData(m_uiVBO, 0, sizeof(m_vecVertices[0]) * m_vecVertices.size(), m_vecVertices.data());
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);  // Bind the vertex buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vecVertices[0]) * m_vecVertices.size(), m_vecVertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind after update
	}
}

GLint CQuadList::GetWidth() const
{
	return (m_iWidth);
}

GLint CQuadList::GetDepth() const
{
	return (m_iDepth);
}

const std::vector<CQuadList::TQuadVertex>& CQuadList::GetVertices() const
{
	return (m_vecVertices);
}

const std::vector<GLuint>& CQuadList::GetIndices() const
{
	return (m_vecIndices);
}

void CQuadList::ApplyTerrainBrush_World(EBrushType eBrushType, float worldX, float worldZ, float fRadius, float fStrength, float fWorldScale)
{
	GLfloat gridX = worldX / fWorldScale;
	GLfloat gridZ = worldZ / fWorldScale;

	GLint startX = std::max(0, static_cast<GLint>(gridX - fRadius / fWorldScale));
	GLint endX = std::min(m_iWidth - 1, static_cast<GLint>(gridX + fRadius / fWorldScale));
	GLint startZ = std::max(0, static_cast<GLint>(gridZ - fRadius / fWorldScale));
	GLint endZ = std::min(m_iDepth - 1, static_cast<GLint>(gridZ + fRadius / fWorldScale));

	float baseHeight = 0.0f;

	// Sample base height if flattening
	if (eBrushType == BRUSH_TYPE_FLATTEN)
	{
		GLint centerX = static_cast<GLint>(gridX);
		GLint centerZ = static_cast<GLint>(gridZ);
		GLint centerIndex = centerZ * m_iWidth + centerX;
		if (centerIndex >= 0 && centerIndex < m_vecVertices.size())
		{
			baseHeight = m_vecVertices[centerIndex].m_v3Pos.y;
		}
	}

	for (GLint z = startZ; z <= endZ; ++z)
	{
		for (GLint x = startX; x <= endX; ++x)
		{
			GLfloat vx = x * fWorldScale;
			GLfloat vz = z * fWorldScale;

			GLfloat dist = glm::distance(glm::vec2(worldX, worldZ), glm::vec2(vx, vz));
			if (dist <= fRadius)
			{
				GLfloat normalizedDist = dist / fRadius;
				GLfloat quadFalloff = 1.0f - (normalizedDist * normalizedDist);
				GLfloat cosFalloff = 0.5f * (1.0f + std::cos(normalizedDist * glm::pi<GLfloat>()));
				GLfloat falloff = 0.7f * quadFalloff + 0.3f * cosFalloff;

				GLint index = z * m_iWidth + x;
				assert(index < m_vecVertices.size());

				GLfloat& currentHeight = m_vecVertices[index].m_v3Pos.y;

				if (eBrushType == BRUSH_TYPE_UP)
				{
					GLfloat heightChange = fStrength * falloff;
					GLfloat rand = 0.05f * (std::sin(x * 0.1f) + std::cos(z * 0.1f));
					heightChange += rand * fStrength * falloff;
					currentHeight += heightChange;
				}
				else if (eBrushType == BRUSH_TYPE_DOWN)
				{
					GLfloat heightChange = fStrength * falloff;
					currentHeight -= heightChange;
				}
				else if (eBrushType == BRUSH_TYPE_FLATTEN)
				{
					currentHeight = glm::mix(currentHeight, baseHeight, falloff);
				}
				else if (eBrushType == BRUSH_TYPE_SMOOTH)
				{
					// Simple 3x3 average smoothing
					GLfloat sum = 0.0f;
					GLint count = 0;

					for (GLint dz = -1; dz <= 1; ++dz)
					{
						for (GLint dx = -1; dx <= 1; ++dx)
						{
							GLint nx = x + dx;
							GLint nz = z + dz;
							if (nx >= 0 && nx < m_iWidth && nz >= 0 && nz < m_iDepth)
							{
								GLint ni = nz * m_iWidth + nx;
								sum += m_vecVertices[ni].m_v3Pos.y;
								count++;
							}
						}
					}

					if (count > 0)
					{
						GLfloat avg = sum / count;
						currentHeight = glm::mix(currentHeight, avg, falloff);
					}
				}
				else if (eBrushType == BRUSH_TYPE_NOISE)
				{
					GLfloat noise = glm::fract(std::sin(x * 12.9898f + z * 78.233f) * 43758.5453f); // Random-ish
					GLfloat heightChange = (noise - 0.5f) * 2.0f * fStrength * falloff;
					currentHeight += heightChange;
				}
			}
		}
	}
	UpdateVertexBuffer();
}
