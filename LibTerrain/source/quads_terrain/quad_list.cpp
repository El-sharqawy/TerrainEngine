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
	m_pTerrain = nullptr;
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
	m_pTerrain = pTerrain;

	sys_log("CQuadList::CreateQuadList: Create Quad List Width: %d, Depth: %d", iWidth, iDepth);

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
		const GLint iNormals = 1;  // Texture coordinate attribute location
		const GLint iTex = 2;  // Texture coordinate attribute location
		const GLint iBlend = 3; // Textures Blending Weights attribute location

		// Enable vertex attributes
		glEnableVertexArrayAttrib(m_uiVAO, iPos);
		glEnableVertexArrayAttrib(m_uiVAO, iTex);
		glEnableVertexArrayAttrib(m_uiVAO, iBlend);

		size_t sNumFlots = 0;

		// Set up vertex attribute formats using DSA
		// Position attribute (location = 0) V3
		glVertexArrayAttribFormat(m_uiVAO, iPos, 3, GL_FLOAT, GL_FALSE, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iPos, 0);

		// vec3 Position
		sNumFlots += 3;

		// Normals attribute (location = 1) V3
		glVertexArrayAttribFormat(m_uiVAO, iNormals, 3, GL_FLOAT, GL_FALSE, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iNormals, 0);

		// vec3 Normals
		sNumFlots += 3;

		// Texture coordinate attribute (location = 2) V2
		glVertexArrayAttribFormat(m_uiVAO, iTex, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iTex, 0);

		// vec2 TexCoords
		sNumFlots += 2;

		// Texture Blending Weights attribute (location = 3) V4
		glVertexArrayAttribFormat(m_uiVAO, iBlend, 4, GL_FLOAT, GL_FALSE, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iBlend, 0);

		// vec4 BlendingWeights
		sNumFlots += 4;

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
		const GLint iNormals = 1;
		const GLint iTex = 2;
		const GLint iBlend = 3; // Textures Blending Weights attribute location

		size_t sNumFlots = 0;

		// Position attribute (location = 0)
		glEnableVertexAttribArray(iPos);
		glVertexAttribPointer(iPos, 3, GL_FLOAT, GL_FALSE, sizeof(TQuadVertex), (const void*)(sNumFlots * sizeof(float)));

		// vec3 Position
		sNumFlots += 3;

		// Normals attribute (location = 1)
		glEnableVertexAttribArray(iNormals);
		glVertexAttribPointer(iNormals, 3, GL_FLOAT, GL_FALSE, sizeof(TQuadVertex), (const void*)(sNumFlots * sizeof(float)));

		// vec3 Normals
		sNumFlots += 3;

		// Texture coordinate attribute (location = 2)
		glEnableVertexAttribArray(iTex);
		glVertexAttribPointer(iTex, 2, GL_FLOAT, GL_FALSE, sizeof(TQuadVertex), (const void*)(sNumFlots * sizeof(float)));

		// vec2 TexCoords
		sNumFlots += 2;

		// Textures Blending Weights attribute (location = 3)
		glEnableVertexAttribArray(iBlend);
		glVertexAttribPointer(iBlend, 4, GL_FLOAT, GL_FALSE, sizeof(TQuadVertex), (const void*)(sNumFlots * sizeof(float)));

		// vec4 BlendingWeights
		sNumFlots += 4;
	}
}

SVector3Df CQuadList::SQuadVertex::CalculateNormal(int x, int z, const float* heightmap, int width, int height)
{
	auto getHeight = [&](int x, int z) -> float
		{
			x = std::clamp(x, 0, width - 1);
			z = std::clamp(z, 0, height - 1);
			return heightmap[z * width + x];
		};

	float heightL = getHeight(x - 1, z);
	float heightR = getHeight(x + 1, z);
	float heightD = getHeight(x, z - 1);
	float heightU = getHeight(x, z + 1);

	SVector3Df normal = SVector3Df(
		heightL - heightR, // X
		2.0f,              // Y up scale (important)
		heightD - heightU  // Z
	);

	return normal.normalize();
}

void CQuadList::SQuadVertex::InitQuadVertex(const CBaseTerrain* pTerrain, GLint iWidth, GLint iDepth, GLint iX, GLint iZ)
{
	float WorldScale = pTerrain->GetWorldScale();
	float fHeight = pTerrain->GetHeight(iX, iZ);
	m_v3Pos = SVector3Df(iX * WorldScale, 0.0f, iZ * WorldScale);

	float TextureScale = pTerrain->GetTextureScale();
	m_v2TexPos = SVector2Df(TextureScale * (float)iX / (float)iWidth, TextureScale * (float)iZ / (float)iDepth);

	m_v4BlendWeights = SVector4Df(0.0f, 0.0f, 0.0f, 0.0f);
	m_v3Normals = SVector3Df(0.0f, 0.0f, 0.0f);
}

void CQuadList::PopulateBuffers(const CBaseTerrain* pTerrain)
{
	size_t numVertices = static_cast<size_t>(m_iWidth * m_iDepth);
	m_vecVertices.resize(numVertices);

	InitVertices(pTerrain);

	size_t NumQuads = static_cast<size_t>((m_iWidth - 1) * (m_iDepth - 1));
	m_vecIndices.resize(NumQuads * 4);
	InitIndices();

	//CalculateNormals();

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

void CQuadList::CalculateNormals()
{
	// Loop over each quad
	for (size_t i = 0; i + 3 < m_vecIndices.size(); i += 4)
	{
		GLuint i0 = m_vecIndices[i + 0];
		GLuint i1 = m_vecIndices[i + 1];
		GLuint i2 = m_vecIndices[i + 2];
		GLuint i3 = m_vecIndices[i + 3];

		const SVector3Df& p0 = m_vecVertices[i0].m_v3Pos;
		const SVector3Df& p1 = m_vecVertices[i1].m_v3Pos;
		const SVector3Df& p2 = m_vecVertices[i2].m_v3Pos;
		const SVector3Df& p3 = m_vecVertices[i3].m_v3Pos;

		// Each quad is split into two triangles: (p0, p1, p2) and (p2, p1, p3)

		// Triangle 1 normal
		SVector3Df edge1 = p1 - p0;
		SVector3Df edge2 = p2 - p0;
		SVector3Df normal1 = edge1.cross(edge2);
		normal1.normalize();

		// Triangle 2 normal
		SVector3Df edge3 = p3 - p1;
		SVector3Df edge4 = p2 - p1;
		SVector3Df normal2 = edge3.cross(edge4);
		normal2.normalize();

		// Accumulate normals for the vertices
		m_vecVertices[i0].m_v3Normals += normal1;
		m_vecVertices[i1].m_v3Normals += normal1;
		m_vecVertices[i2].m_v3Normals += normal1;

		m_vecVertices[i1].m_v3Normals += normal2;
		m_vecVertices[i2].m_v3Normals += normal2;
		m_vecVertices[i3].m_v3Normals += normal2;
	}

	// Normalize all the accumulated normals
	for (auto& vertex : m_vecVertices)
	{
		vertex.m_v3Normals.normalize();
	}

	UpdateVertexBuffer();
}

void CQuadList::Render() const
{
	glBindVertexArray(m_uiVAO);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glDrawElements(GL_PATCHES, (m_iDepth - 1) * (m_iWidth - 1) * 4, GL_UNSIGNED_INT, NULL);
	//glDrawElementsInstanced(GL_PATCHES, (m_iDepth - 1) * (m_iWidth - 1) * 4, GL_UNSIGNED_INT, 0, m_vecVertices.size());
	glBindVertexArray(0);
}

void CQuadList::UpdateVertexBuffer()
{
	if (IsGLVersionHigher(4, 5))
	{
		// Update vertex data using DSA
		glNamedBufferSubData(m_uiVBO, 0, sizeof(TQuadVertex) * m_vecVertices.size(), m_vecVertices.data());
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);  // Bind the vertex buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TQuadVertex) * m_vecVertices.size(), m_vecVertices.data());
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

bool CQuadList::GetRayTerrainIntersection(const SVector3Df& rayOrigin, const SVector3Df& rayDir, SVector3Df& intersectionPoint)
{
	float  bestT = std::numeric_limits<float>::infinity();
	SVector3Df bestHit;
	bool   hitAny = false;

	for (size_t i = 0; i + 3 < m_vecIndices.size(); i += 4)
	{
		const auto& V = m_vecVertices;
		SVector3Df v0 = V[m_vecIndices[i + 0]].m_v3Pos;
		SVector3Df v1 = V[m_vecIndices[i + 1]].m_v3Pos;
		SVector3Df v2 = V[m_vecIndices[i + 2]].m_v3Pos;
		SVector3Df v3 = V[m_vecIndices[i + 3]].m_v3Pos;

		SVector3Df P;
		float t;
		if (IntersectQuad(rayOrigin, rayDir, v0, v1, v2, v3, P, t) && t < bestT)
		{
			bestT = t;
			bestHit = P;
			hitAny = true;
		}
	}

	if (hitAny)
	{
		intersectionPoint = bestHit;
		return true;
	}
	return false;
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

	std::vector<GLfloat> originalHeights(m_vecVertices.size());
	for (size_t i = 0; i < m_vecVertices.size(); ++i)
		originalHeights[i] = m_vecVertices[i].m_v3Pos.y;

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
					float alpha = falloff * falloff; // Apply a curve to falloff
					currentHeight = glm::mix(currentHeight, baseHeight, alpha);
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
								//sum += originalHeights[ni]; // Use originalHeights
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

/*void CQuadList::ApplyTextureBrush_Circle(float worldX, float worldZ, float fRadius, BYTE textureID, bool bErase, bool bDrawOnlyOnBlankTile)
{
	float fWorldScale = 1.0f; // Change this if your terrain has a scale
	float gridX = worldX / fWorldScale;
	float gridZ = worldZ / fWorldScale;

	GLint startX = std::max(0, static_cast<GLint>(gridX - fRadius / fWorldScale));
	GLint endX = std::min(m_iWidth - 1, static_cast<GLint>(gridX + fRadius / fWorldScale));
	GLint startZ = std::max(0, static_cast<GLint>(gridZ - fRadius / fWorldScale));
	GLint endZ = std::min(m_iDepth - 1, static_cast<GLint>(gridZ + fRadius / fWorldScale));

	for (GLint z = startZ; z <= endZ; ++z)
	{
		for (GLint x = startX; x <= endX; ++x)
		{
			GLfloat vx = x * fWorldScale;
			GLfloat vz = z * fWorldScale;

			GLfloat dist = glm::distance(glm::vec2(worldX, worldZ), glm::vec2(vx, vz));
			if (dist <= fRadius)
			{
				GLint index = z * m_iWidth + x;
				assert(index < m_vecVertices.size());

				BYTE& currentTexID = m_vecVertices[index].m_TextureID;

				if (bDrawOnlyOnBlankTile)
				{
					if (currentTexID == 0)
					{
						currentTexID = textureID;
					}
				}
				else
				{
					if (bErase)
					{
						if (currentTexID == textureID)
							currentTexID = 0;
					}
					else
					{
						currentTexID = textureID;
					}
				}
			}
		}
	}
	UpdateVertexBuffer();
}*/

void CQuadList::ApplyTextureBrush(float worldX, float worldZ, float radius, int textureLayer, float strength, float fWorldScale, bool additive)
{
	float gridX = worldX / fWorldScale;
	float gridZ = worldZ / fWorldScale;

	GLint startX = std::max(0, static_cast<GLint>(gridX - radius / fWorldScale));
	GLint endX = std::min(m_iWidth - 1, static_cast<GLint>(gridX + radius / fWorldScale));
	GLint startZ = std::max(0, static_cast<GLint>(gridZ - radius / fWorldScale));
	GLint endZ = std::min(m_iDepth - 1, static_cast<GLint>(gridZ + radius / fWorldScale));

	// DEBUG: Print first modified vertex
	for (GLint z = startZ; z <= endZ; ++z)
	{
		for (GLint x = startX; x <= endX; ++x)
		{
			GLfloat vx = x * fWorldScale;
			GLfloat vz = z * fWorldScale;

			GLfloat dist = glm::distance(glm::vec2(worldX, worldZ), glm::vec2(vx, vz));
			if (dist <= radius)
			{
				GLfloat falloff = 1.0f - (dist / radius);
				falloff = glm::clamp(falloff * strength, 0.0f, 1.0f);

				GLint index = z * m_iWidth + x;
				if (index >= m_vecVertices.size()) continue;

				SVector4Df& blend = m_vecVertices[index].m_v4BlendWeights;
				if (textureLayer < 0 || textureLayer >= 4)
					continue;

				int pixelIndex = (z * m_iWidth + x) * 4;

				// Apply to blend weights
				if (additive)
				{
					blend[textureLayer] = glm::min(blend[textureLayer] + (falloff * strength), 1.0f);
				}
				else
				{
					blend[textureLayer] = falloff;
				}

				sys_err("Blend weights: %.2f, %.2f, %.2f, %.2f\n",
					m_vecVertices[index].m_v4BlendWeights.x, m_vecVertices[index].m_v4BlendWeights.y, m_vecVertices[index].m_v4BlendWeights.z, m_vecVertices[index].m_v4BlendWeights.w);

				UpdateVertexBuffer();

				// Optional: Normalize weights (uncomment if needed)
				// float total = blend.x + blend.y + blend.z + blend.w;
				// if (total > 1.0f) blend /= total;
			}
		}
	}
}

void CQuadList::ApplyHeightmap(const GLfloat* pHeightMap, GLint mapWidth, GLint mapHeight)
{
    for (auto& vertex : m_vecVertices)
    {
        // Assuming world space X and Z are 0..TerrainSize
        float u = vertex.m_v3Pos.x / (float)(mapWidth - 1);
        float v = vertex.m_v3Pos.z / (float)(mapHeight - 1);

        u = std::clamp(u, 0.0f, 1.0f);
        v = std::clamp(v, 0.0f, 1.0f);

        int x = static_cast<int>(u * (mapWidth - 1));
        int y = static_cast<int>(v * (mapHeight - 1));

        int index = y * mapWidth + x;
        float height = pHeightMap[index];

        vertex.m_v3Pos.y = height;
    }

	UpdateVertexBuffer();
}
