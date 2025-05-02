#include "stdafx.h"
#include "geomip_grid.h"
#include "terrain.h"
#include <algorithm>

#if defined(_WIN64)
#undef max
#undef min
#undef minmax
#endif

CGeoMipGrid::CGeoMipGrid()
{
	m_iWidth = 0;
	m_iDepth = 0;
	m_uiVAO = 0;
	m_uiVBO = 0;
	m_uiIdxBuf = 0;
	m_iMaxLOD = 0;
	m_iPatchSize = 0;
	m_iMaxLOD = 0;
	m_iNumPatchesX = 0;
	m_iNumPatchesZ = 0;
	m_pTerrain = nullptr;
	m_fWorldScale = 1.0f;
	m_iCurTextureIndex = 0;
}

CGeoMipGrid::~CGeoMipGrid()
{
	Destroy();
}

void CGeoMipGrid::Destroy()
{
	if (m_uiVAO)
	{
		glDeleteVertexArrays(1, &m_uiVAO);
	}
	if (m_uiVBO)
	{
		glDeleteBuffers(1, &m_uiVBO);
	}
	if (m_uiIdxBuf)
	{
		glDeleteBuffers(1, &m_uiIdxBuf);
	}

	// Clear Splats Data
	for (auto& tex : m_vSplatGLTextures)
	{
		if (tex)
		{
			safe_delete(tex);
		}
	}

	m_vSplatBindings.clear();
	m_vSplatGLTextures.clear();
	m_vSplatData.clear();
}

void CGeoMipGrid::CreateGeoMipGrid(GLint iWidth, GLint iDepth, GLint iPatchSize, CBaseTerrain* pTerrain)
{
	if ((iWidth - 1) % (iPatchSize - 1) != 0)
	{
		GLint iRecommendedWidth = ((iWidth - 1 + iPatchSize - 1) / (iPatchSize - 1)) * (iPatchSize - 1) + 1;
		sys_err("Width minus 1 (%d) must be divisible by Patchsize minus 1 (%d)", iWidth, iPatchSize);
		sys_err("Try using this recommended Width: %d", iRecommendedWidth);
		return;
	}

	if ((iDepth - 1) % (iPatchSize - 1) != 0)
	{
		GLint iRecommendedDepth = ((iDepth - 1 + iPatchSize - 1) / (iPatchSize - 1)) * (iPatchSize - 1) + 1;
		sys_err("Depth minus 1 (%d) must be divisible by Patchsize minus 1 (%d)", iDepth, iPatchSize);
		sys_err("Try using this recommended Depth: %d", iRecommendedDepth);
		return;
	}
	if (iPatchSize < 3)
	{
		sys_err("Minimum Patchsize is 3 (%d)", iPatchSize);
		return;
	}

	if (iPatchSize % 2 == 0)
	{
		sys_err("Patchsize must be an odd number! (%d)", iPatchSize);
		return;
	}

	m_iWidth = iWidth;
	m_iDepth = iDepth;
	m_iPatchSize = iPatchSize;
	m_pTerrain = pTerrain;

	m_iNumPatchesX = (iWidth - 1) / (iPatchSize - 1);
	m_iNumPatchesZ = (iDepth - 1) / (iPatchSize - 1);

	m_fWorldScale = pTerrain->GetWorldTranslation()->GetScale();
	m_iMaxLOD = CLodManager::Instance().InitLodManager(iPatchSize, m_iNumPatchesX, m_iNumPatchesZ, m_fWorldScale);
	m_vLodInfo.resize(m_iMaxLOD + 1);

	CreateGLState();
	PopulateBuffers(pTerrain);
	SetupSplatTextures();
	UploadSplatBindings();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	sys_log("CGeoMipGrid::CreateTriangleList Created Triangle List Size: %d, Width: %d, Depth: %d", iWidth * iDepth, iWidth, iDepth);
}

void CGeoMipGrid::CreateGLState()
{
	if (IsGLVersionHigher(4, 5))
	{
		// Create and bind VAO
		glCreateVertexArrays(1, &m_uiVAO);

		// Create VBO using DSA (Direct State Access)
		glCreateBuffers(1, &m_uiVBO);

		// Create IBO using DSA
		glCreateBuffers(1, &m_uiIdxBuf);

		// Set up vertex attributes using DSA
		const GLint iPos = 0;  // Position attribute location
		const GLint iTex = 1;  // Texture coordinate attribute location
		const GLint iNormals = 2;  // Texture coordinate attribute location
		const GLint iTextureIndex = 3;  // Texture Index attribute location
		const GLint iTextureBlendFactor = 4;  // Texture Blend Factor attribute location

		// Bind the VBO to the VAO's binding point 0
		glVertexArrayVertexBuffer(m_uiVAO, 0, m_uiVBO, 0, sizeof(TVertex));

		// Bind the IBO to the VAO
		glVertexArrayElementBuffer(m_uiVAO, m_uiIdxBuf);

		// Enable vertex attributes
		glEnableVertexArrayAttrib(m_uiVAO, iPos);
		glEnableVertexArrayAttrib(m_uiVAO, iTex);
		glEnableVertexArrayAttrib(m_uiVAO, iNormals);
		glEnableVertexArrayAttrib(m_uiVAO, iTextureIndex);
		glEnableVertexArrayAttrib(m_uiVAO, iTextureBlendFactor);

		size_t sNumFlots = 0;

		// Set up vertex attribute formats using DSA
		// Position attribute (location = 0) V3
		glVertexArrayAttribFormat(m_uiVAO, iPos, 3, GL_FLOAT, GL_FALSE, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iPos, 0);

		// vec3 Position
		sNumFlots += 3;

		// Texture coordinate attribute (location = 1) V2
		glVertexArrayAttribFormat(m_uiVAO, iTex, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iTex, 0);

		// vec2 TexCoords
		sNumFlots += 2;

		// Normals attribute (location = 2) V3
		glVertexArrayAttribFormat(m_uiVAO, iNormals, 3, GL_FLOAT, GL_FALSE, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iNormals, 0);

		// vec3 Normals
		sNumFlots += 3;

		glVertexArrayAttribIFormat(m_uiVAO, iTextureIndex, 1, GL_INT, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iTextureIndex, 0);

		sNumFlots += 1;

		glVertexArrayAttribFormat(m_uiVAO, iTextureBlendFactor, 1, GL_FLOAT, GL_FALSE, static_cast<GLuint>(sNumFlots * sizeof(GLfloat)));
		glVertexArrayAttribBinding(m_uiVAO, iTextureBlendFactor, 0);

		sNumFlots += 1;
	}
	else
	{
		glGenVertexArrays(1, &m_uiVAO);
		glBindVertexArray(m_uiVAO);

		glGenBuffers(1, &m_uiVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);

		glGenBuffers(1, &m_uiIdxBuf);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIdxBuf);

		const GLint POS_LOC = 0;
		const GLint	TEX_LOC = 1;
		const GLint	NORMALS_LOC = 2;
		const GLint iTextureIndex = 3;  // Texture Index attribute location

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
		glEnableVertexAttribArray(iTextureIndex);
		glVertexAttribIPointer(iTextureIndex, 1, GL_INT, sizeof(TVertex), (const void*)(NumFloats * sizeof(float)));
		NumFloats += 1;
	}
}

void CGeoMipGrid::PopulateBuffers(CBaseTerrain* pTerrain)
{
	m_vecVertices.resize(m_iWidth * m_iDepth);

#if defined(_DEBUG)
	sys_log("Preparing Space for %zu Vertices", m_vecVertices.size());
#endif

	InitVertices(pTerrain);

	GLint iNumIndices = CalculateNumIndices();
	m_vecIndices.resize(iNumIndices);
	iNumIndices = InitIndices();
#if defined(_DEBUG)
	sys_log("Final number of indices %d ", iNumIndices);
#endif

	CalculateNormals();

	if (IsGLVersionHigher(4, 5))
	{
		// Upload vertex data using DSA
		glNamedBufferData(m_uiVBO, sizeof(m_vecVertices[0]) * m_vecVertices.size(), m_vecVertices.data(), GL_STATIC_DRAW);

		// Upload index data using DSA
		glNamedBufferData(m_uiIdxBuf, sizeof(m_vecIndices[0]) * m_vecIndices.size(), m_vecIndices.data(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vecVertices[0]) * m_vecVertices.size(), &m_vecVertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vecIndices[0]) * iNumIndices, &m_vecIndices[0], GL_STATIC_DRAW);
	}
}

GLint CGeoMipGrid::CalculateNumIndices() const
{
	GLint iNumQuads = (m_iPatchSize - 1) * (m_iPatchSize - 1);
	GLint iNumIndices = 0;

	GLint iMaxPermutationsPerLevel = 16;	// true/false for each of the four sides
	const GLint iIndicesPerQuad = 6;		// two triangles

	for (GLint iLOD = 0; iLOD <= m_iMaxLOD; iLOD++)
	{
#if defined(_DEBUG)
		sys_log("LOD %d: num quads %d", iLOD, iNumQuads);
#endif
		iNumIndices += iNumQuads * iIndicesPerQuad * iMaxPermutationsPerLevel;
		iNumQuads /= 4;
	}

#if defined(_DEBUG)
	sys_log("Initial number of indices %d", iNumIndices);
#endif
	return (iNumIndices);
}

void CGeoMipGrid::TVertex::InitVertex(CBaseTerrain* pTerrain, GLint x, GLint z)
{
	SVector3Df pos = pTerrain->GetWorldTranslation()->GetPosition();

	const float y = pTerrain->GetHeight(x, z);
	const float WorldScale = pTerrain->GetWorldScale();
	m_v3Pos = SVector3Df(x * WorldScale, y, z * WorldScale);

	const float fSize = static_cast<float>(pTerrain->GetSize());
	const float TextureScale = pTerrain->GetTextureScale();
	const float fXCoord = TextureScale * x / fSize;
	const float fzCoord = TextureScale * z / fSize;
	m_v2TexCoords = SVector2Df(fXCoord, fzCoord);
	m_v3Normals = SVector3Df(0.0f);
}

void CGeoMipGrid::InitVertices(CBaseTerrain* pTerrain)
{
	GLint Index = 0;

	for (GLint z = 0; z < m_iDepth; z++)
	{
		for (GLint x = 0; x < m_iWidth; x++)
		{
			assert(Index < m_vecVertices.size());
			m_vecVertices[Index].InitVertex(pTerrain, x, z);
			Index++;
		}
	}

	assert(Index == m_vecVertices.size());
}

GLint CGeoMipGrid::InitIndices()
{
	GLuint Index = 0;
	
	for (GLint iLOD = 0; iLOD <= m_iMaxLOD; iLOD++)
	{
#if defined(_DEBUG)
		sys_log("** Init Indices LOD %d ***", iLOD);
#endif
		Index += InitIndicesLOD(Index, m_vecIndices, iLOD);
	}

	return (Index);
}

GLint CGeoMipGrid::InitIndicesLOD(GLint Index, std::vector<GLuint>& Indices, GLint iLOD)
{
	GLint iTotalIndices = 0;

	for (GLint iL = 0; iL < LEFT; iL++)
	{
		for (GLint iR = 0; iR < RIGHT; iR++)
		{
			for (GLint iT = 0; iT < TOP; iT++)
			{
				for (GLint iB = 0; iB < BOTTOM; iB++)
				{
					m_vLodInfo[iLOD].LodInfo[iL][iR][iT][iB].iStart = Index;

					Index = InitIndicesLODSingle(Index, Indices, iLOD, iLOD + iL, iLOD + iR, iLOD + iT, iLOD + iB);

					m_vLodInfo[iLOD].LodInfo[iL][iR][iT][iB].iCount = Index - m_vLodInfo[iLOD].LodInfo[iL][iR][iT][iB].iStart;
					iTotalIndices += m_vLodInfo[iLOD].LodInfo[iL][iR][iT][iB].iCount;
				}
			}
		}
	}

#if defined(_DEBUG)
	sys_log("** Total Indices for LOD %d is: %d ***", iLOD, iTotalIndices);
#endif
	return (iTotalIndices);
}

GLint CGeoMipGrid::InitIndicesLODSingle(GLint Index, std::vector<GLuint>& Indices, GLint iCoreLOD, GLint iLeftLOD, GLint iRightLOD, GLint iTopLOD, GLint iBottomLOD)
{
	const GLint iFanStep = powi(2, iCoreLOD + 1);		// lod = 0 --> 2, lod = 1 --> 4, lod = 2 --> 8, etc
	const GLint iEndPos = m_iPatchSize - 1 - iFanStep;	// patch size 5, fan step 2 --> EndPos = 2; patch size 9, fan step 2 --> EndPos = 6

	for (GLint z = 0; z <= iEndPos; z += iFanStep)
	{
		for (GLint x = 0; x <= iEndPos; x += iFanStep)
		{
			// if X == 0, il = LEFT, otherwise il = iCoreLOD
			GLint iLeft		= x == 0 ? iLeftLOD : iCoreLOD;
			GLint iRight	= x == iEndPos ? iRightLOD : iCoreLOD;
			GLint iBottom	= z == 0 ? iBottomLOD : iCoreLOD;
			GLint iTop		= z == iEndPos ? iTopLOD : iCoreLOD;

			Index = CreateTriangleFan(Index, Indices, iCoreLOD, iLeft, iRight, iTop, iBottom, x, z);
		}
	}

	return (Index);
}

GLuint CGeoMipGrid::CreateTriangleFan(GLint Index, std::vector<GLuint>& Indices, GLint iCoreLOD, GLint iLeftLOD, GLint iRightLOD, GLint iTopLOD, GLint iBottomLOD, GLint iX, GLint iZ)
{
	GLint iStepLeft = powi(2, iLeftLOD); // because LOD starts at zero...
	GLint iStepRight = powi(2, iRightLOD);
	GLint iStepTop = powi(2, iTopLOD);
	GLint iStepBottom = powi(2, iBottomLOD);
	GLint iStepCenter = powi(2, iCoreLOD);

	GLuint IndexCenter = (iZ + iStepCenter) * m_iWidth + iX + iStepCenter;

	// First UP
	GLuint IndexTemp1 = iZ * m_iWidth + iX;
	GLuint IndexTemp2 = (iZ + iStepLeft) * m_iWidth + iX;

	Index = AddTriangle(Index, Indices, IndexCenter, IndexTemp1, IndexTemp2);

	// Second UP
	if (iLeftLOD == iCoreLOD)
	{
		IndexTemp1 = IndexTemp2;
		IndexTemp2 += iStepLeft * m_iWidth;

		Index = AddTriangle(Index, Indices, IndexCenter, IndexTemp1, IndexTemp2);
	}

	// First Right
	IndexTemp1 = IndexTemp2;
	IndexTemp2 += iStepTop;

	Index = AddTriangle(Index, Indices, IndexCenter, IndexTemp1, IndexTemp2);

	// Second Right
	if (iTopLOD == iCoreLOD)
	{
		IndexTemp1 = IndexTemp2;
		IndexTemp2 += iStepTop;

		Index = AddTriangle(Index, Indices, IndexCenter, IndexTemp1, IndexTemp2);
	}

	// First Down
	IndexTemp1 = IndexTemp2;
	IndexTemp2 -= iStepRight * m_iWidth;

	Index = AddTriangle(Index, Indices, IndexCenter, IndexTemp1, IndexTemp2);

	// Second Down
	if (iRightLOD == iCoreLOD)
	{
		IndexTemp1 = IndexTemp2;
		IndexTemp2 -= iStepRight * m_iWidth;

		Index = AddTriangle(Index, Indices, IndexCenter, IndexTemp1, IndexTemp2);
	}

	// First Left
	IndexTemp1 = IndexTemp2;
	IndexTemp2 -= iStepBottom;

	Index = AddTriangle(Index, Indices, IndexCenter, IndexTemp1, IndexTemp2);

	// Second Left
	if (iBottomLOD == iCoreLOD)
	{
		IndexTemp1 = IndexTemp2;
		IndexTemp2 -= iStepBottom;

		Index = AddTriangle(Index, Indices, IndexCenter, IndexTemp1, IndexTemp2);
	}

	return (Index);
}

GLuint CGeoMipGrid::AddTriangle(GLuint uiIndex, std::vector<GLuint>& vIndicies, GLuint v1, GLuint v2, GLuint v3)
{
	assert(uiIndex < vIndicies.size());
	vIndicies[uiIndex++] = v1;

	assert(uiIndex < vIndicies.size());
	vIndicies[uiIndex++] = v2;

	assert(uiIndex < vIndicies.size());
	vIndicies[uiIndex++] = v3;
	
	return (uiIndex);
}

void CGeoMipGrid::CalculateNormals()
{
	GLuint dwIndex = 0;

	// Accumulate each triangle normal into each of triangle vertices

	for (GLint z = 0; z < m_iDepth - 1; z += (m_iPatchSize - 1))
	{
		for (GLint x = 0; x < m_iWidth - 1; x += (m_iPatchSize - 1))
		{
			GLint iBaseVertex = z * m_iWidth + x;
			GLint iNumIndices = m_vLodInfo[0].LodInfo[0][0][0][0].iCount;

			for (GLint i = 0; i < iNumIndices; i += 3)
			{
				GLuint uiIndex0 = iBaseVertex + m_vecIndices[i];
				GLuint uiIndex1 = iBaseVertex + m_vecIndices[i + 1];
				GLuint uiIndex2 = iBaseVertex + m_vecIndices[i + 2];

				SVector3Df v1 = m_vecVertices[uiIndex1].m_v3Pos - m_vecVertices[uiIndex0].m_v3Pos;
				SVector3Df v2 = m_vecVertices[uiIndex2].m_v3Pos - m_vecVertices[uiIndex0].m_v3Pos;

				SVector3Df v3Normals = v1.cross(v2);

				v3Normals.normalize();

				m_vecVertices[uiIndex0].m_v3Normals += v3Normals;
				m_vecVertices[uiIndex1].m_v3Normals += v3Normals;
				m_vecVertices[uiIndex2].m_v3Normals += v3Normals;
			}
		}
	}

	// Normalize all the vertex normals
	for (GLuint i = 0; i < m_vecVertices.size(); i++)
	{
		m_vecVertices[i].m_v3Normals.normalize();
	}
}

void CGeoMipGrid::Render()
{
	if (m_uiVAO == 0 || m_uiVBO == 0 || m_uiIdxBuf == 0)
	{
		printf("Failed to create VAO, VBO, or Index Buffer.\n");
		exit(EXIT_FAILURE);
	}

	CLodManager::Instance().Update();

	// Bind SSBO to index 0
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_uiSplatTerrainHandlesSSBO);

	glBindVertexArray(m_uiVAO);

	for (GLint iPatchZ = 0; iPatchZ < m_iNumPatchesZ; iPatchZ++)
	{
		for (GLint iPatchX = 0; iPatchX < m_iNumPatchesX; iPatchX++)
		{
			GLint iX = iPatchX * (m_iPatchSize - 1);
			GLint iZ = iPatchZ * (m_iPatchSize - 1);

			SFrustumCulling sFC(CCameraManager::Instance().GetCurrentCamera()->GetViewProjMatrix());

			if (!IsPatchInsideViewFrustumWorldSpace(iX, iZ, sFC))
			{
				continue;
			}

			const TPatchLod& pLOD = CLodManager::Instance().GetPatchLod(iPatchX, iPatchZ);
			GLint iCore = pLOD.iCore;
			GLint iLeft = pLOD.iLeft;
			GLint iRight = pLOD.iRight;
			GLint iTop = pLOD.iTop;
			GLint iBottom = pLOD.iBottom;

			size_t sBaseIndex = sizeof(GLuint) * m_vLodInfo[iCore].LodInfo[iLeft][iRight][iTop][iBottom].iStart;

			GLint iBaseVertex = iZ * m_iWidth + iX;

			glDrawElementsBaseVertex(GL_TRIANGLES, m_vLodInfo[iCore].LodInfo[iLeft][iRight][iTop][iBottom].iCount, GL_UNSIGNED_INT, (void*)sBaseIndex, iBaseVertex);
		}
	}

	glBindVertexArray(0);
	// Unbind SSBO to index 1
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

}

void CGeoMipGrid::Render(const SVector3Df& CameraPos, const CMatrix4Df& ViewProj)
{
	if (m_uiVAO == 0 || m_uiVBO == 0 || m_uiIdxBuf == 0)
	{
		printf("Failed to create VAO, VBO, or Index Buffer.\n");
		exit(EXIT_FAILURE);
	}

	CLodManager::Instance().Update(CameraPos);

	SFrustumCulling sFC(ViewProj);

	// Bind SSBO to index 0
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_uiSplatTerrainHandlesSSBO);

	glBindVertexArray(m_uiVAO);
	// Set tessellation levels, you may want to adjust these based on your LOD (level of detail)
	glPatchParameteri(GL_PATCH_VERTICES, 3);  // Assuming each patch is a triangle (3 vertices)

	for (GLint iPatchZ = 0; iPatchZ < m_iNumPatchesZ; iPatchZ++)
	{
		for (GLint iPatchX = 0; iPatchX < m_iNumPatchesX; iPatchX++)
		{
			GLint iX = iPatchX * (m_iPatchSize - 1);
			GLint iZ = iPatchZ * (m_iPatchSize - 1);


			if (!IsPatchInsideViewFrustumWorldSpace(iX, iZ, sFC))
			//if (!IsPatchInsideViewFrustumViewSpace(iX, iZ, ViewProj))
			{
				continue;
			}

			const TPatchLod& pLOD = CLodManager::Instance().GetPatchLod(iPatchX, iPatchZ);
			GLint iCore = pLOD.iCore;
			GLint iLeft = pLOD.iLeft;
			GLint iRight = pLOD.iRight;
			GLint iTop = pLOD.iTop;
			GLint iBottom = pLOD.iBottom;

			GLint iPatchIndex = iPatchZ * m_iNumPatchesX + iPatchX;

			m_pTerrain->GetTerrainShader()->Use();
			m_pTerrain->GetTerrainShader()->setInt("iLodLevel", iCore);
			m_pTerrain->GetTerrainShader()->setInt("iLodLevelMax", m_iMaxLOD);

			glActiveTexture(GL_TEXTURE0 + COLOR_TEXTURE_UNIT_INDEX_5);
			glBindTexture(GL_TEXTURE_2D, m_vSplatGLTextures[iPatchIndex]->GetTextureID());

			m_pTerrain->GetTerrainShader()->setInt("splatmap", COLOR_TEXTURE_UNIT_INDEX_5);             // bind sampler
			m_pTerrain->GetTerrainShader()->setInt("iPatchIndex", iPatchIndex); // for bindless IDs

			size_t sBaseIndex = sizeof(GLuint) * m_vLodInfo[iCore].LodInfo[iLeft][iRight][iTop][iBottom].iStart;

			GLint iBaseVertex = iZ * m_iWidth + iX;

			glDrawElementsBaseVertex(GL_PATCHES, m_vLodInfo[iCore].LodInfo[iLeft][iRight][iTop][iBottom].iCount, GL_UNSIGNED_INT, (void*)sBaseIndex, iBaseVertex);

			glActiveTexture(GL_TEXTURE0 + COLOR_TEXTURE_UNIT_INDEX_5);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glBindVertexArray(0);
	// Unbind SSBO to index 1
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
}

bool CGeoMipGrid::IsPatchInsideViewFrustumViewSpace(GLint iX, GLint iZ, const CMatrix4Df& matViewProj) const
{
	GLint iX0 = iX;
	GLint iX1 = iX + m_iPatchSize - 1;
	GLint iZ0 = iZ;
	GLint iZ1 = iZ + m_iPatchSize - 1;

	const SVector3Df point00 = SVector3Df(static_cast<float>(iX0) * m_fWorldScale, m_pTerrain->GetHeight(iX0, iZ0), (static_cast<float>(iZ0) * m_fWorldScale));
	const SVector3Df point01 = SVector3Df(static_cast<float>(iX0) * m_fWorldScale, m_pTerrain->GetHeight(iX0, iZ1), (static_cast<float>(iZ1) * m_fWorldScale));
	const SVector3Df point10 = SVector3Df(static_cast<float>(iX1) * m_fWorldScale, m_pTerrain->GetHeight(iX1, iZ0), (static_cast<float>(iZ0) * m_fWorldScale));
	const SVector3Df point11 = SVector3Df(static_cast<float>(iX1) * m_fWorldScale, m_pTerrain->GetHeight(iX1, iZ1), (static_cast<float>(iZ1) * m_fWorldScale));

	bool bInsideViewFrustum = (
		(IsPointInsideViewFrustum(point00, matViewProj)) ||
		(IsPointInsideViewFrustum(point01, matViewProj)) ||
		(IsPointInsideViewFrustum(point10, matViewProj)) ||
		(IsPointInsideViewFrustum(point11, matViewProj)));

	return (bInsideViewFrustum);
}

bool CGeoMipGrid::IsPatchInsideViewFrustumWorldSpace(GLint iX, GLint iZ, const SFrustumCulling& sFrustumCulling) const
{
	GLint iX0 = iX;
	GLint iX1 = iX + m_iPatchSize - 1;
	GLint iZ0 = iZ;
	GLint iZ1 = iZ + m_iPatchSize - 1;

	float fH00 = m_pTerrain->GetHeight(iX0, iZ0);
	float fH01 = m_pTerrain->GetHeight(iX0, iZ1);
	float fH10 = m_pTerrain->GetHeight(iX1, iZ0);
	float fH11 = m_pTerrain->GetHeight(iX1, iZ1);

	float fMinHeight = std::min(fH00, std::min(fH01, std::min(fH10, fH11)));
	float fMaxHeight = std::max(fH00, std::max(fH01, std::max(fH10, fH11)));

	const SVector3Df point00_low = SVector3Df(static_cast<float>(iX0) * m_fWorldScale, fMinHeight, (static_cast<float>(iZ0) * m_fWorldScale));
	const SVector3Df point01_low = SVector3Df(static_cast<float>(iX0) * m_fWorldScale, fMinHeight, (static_cast<float>(iZ1) * m_fWorldScale));
	const SVector3Df point10_low = SVector3Df(static_cast<float>(iX1) * m_fWorldScale, fMinHeight, (static_cast<float>(iZ0) * m_fWorldScale));
	const SVector3Df point11_low = SVector3Df(static_cast<float>(iX1) * m_fWorldScale, fMinHeight, (static_cast<float>(iZ1) * m_fWorldScale));

	const SVector3Df point00_high = SVector3Df(static_cast<float>(iX0) * m_fWorldScale, fMaxHeight, (static_cast<float>(iZ0) * m_fWorldScale));
	const SVector3Df point01_high = SVector3Df(static_cast<float>(iX0) * m_fWorldScale, fMaxHeight, (static_cast<float>(iZ1) * m_fWorldScale));
	const SVector3Df point10_high = SVector3Df(static_cast<float>(iX1) * m_fWorldScale, fMaxHeight, (static_cast<float>(iZ0) * m_fWorldScale));
	const SVector3Df point11_high = SVector3Df(static_cast<float>(iX1) * m_fWorldScale, fMaxHeight, (static_cast<float>(iZ1) * m_fWorldScale));

	bool bInsideViewFrustum = (
		(sFrustumCulling.IsPointInsideViewFrustum(point00_low)) ||
		(sFrustumCulling.IsPointInsideViewFrustum(point01_low)) ||
		(sFrustumCulling.IsPointInsideViewFrustum(point10_low)) ||
		(sFrustumCulling.IsPointInsideViewFrustum(point11_low)) ||

		(sFrustumCulling.IsPointInsideViewFrustum(point00_high)) ||
		(sFrustumCulling.IsPointInsideViewFrustum(point01_high)) ||
		(sFrustumCulling.IsPointInsideViewFrustum(point10_high)) ||
		(sFrustumCulling.IsPointInsideViewFrustum(point11_high)));

	return (bInsideViewFrustum);
}

void CGeoMipGrid::UpdateVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);  // Bind the vertex buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vecVertices[0]) * m_vecVertices.size(), m_vecVertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind after update
}

std::vector<CGeoMipGrid::TVertex>& CGeoMipGrid::GetVertices()
{
	return m_vecVertices; // Return the vector by reference
}

std::vector<GLuint>& CGeoMipGrid::GetIndices()
{
	return (m_vecIndices);
}

std::vector<TLodInfo>& CGeoMipGrid::GetLodInfo()
{
	return (m_vLodInfo);
}

GLint CGeoMipGrid::GetWidth() const
{
	return (m_iWidth);
}

GLint CGeoMipGrid::GetDepth() const
{
	return (m_iDepth);
}

GLint CGeoMipGrid::GetPatchSize() const
{
	return (m_iPatchSize);
}

GLint CGeoMipGrid::GetMaxLOD() const
{
	return (m_iMaxLOD);
}

GLint CGeoMipGrid::GetNumPatchesX() const
{
	return (m_iNumPatchesX);
}

GLint CGeoMipGrid::GetNumPatchesZ() const
{
	return (m_iNumPatchesZ);
}

GLint CGeoMipGrid::GetCurrentTextureIndex() const
{
	return m_iCurTextureIndex;
}

void CGeoMipGrid::ApplyTerrainBrush_World(EBrushType eBrushType, GLfloat worldX, GLfloat worldZ, GLfloat fRadius, GLfloat fStrength)
{
	GLfloat fWorldScale = m_pTerrain->GetWorldScale();
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
				else if (eBrushType == BRUSH_TYPE_TEXTURE)
				{
					/*TBrushParams brush{};
					brush.v2WorldPos = SVector2Df(worldX, worldZ);
					brush.fRadius = 5.0f;
					brush.fStrength = 1.0f;
					brush.iSelectedTexChannel = 1; // G channel (for grass, e.g.)

					PaintSplatmap(brush);*/
				}
			}
		}
	}
	UpdateNormals();
	UpdateVertexBuffer();
}

void CGeoMipGrid::UpdateNormals()
{
	std::vector<SVector3Df> normals(m_vecVertices.size(), SVector3Df(0.0f));
	int gridWidth = m_iWidth;	/* your grid width */
	int gridHeight = m_iDepth;	/* your grid height */

	for (int z = 0; z < gridHeight; z++) {
		for (int x = 0; x < gridWidth; x++) {
			int idx = z * gridWidth + x;
			SVector3Df pos = m_vecVertices[idx].m_v3Pos;

			// Get neighboring positions (with boundary checks)
			SVector3Df right = (x + 1 < gridWidth) ? m_vecVertices[idx + 1].m_v3Pos : pos;
			SVector3Df left = (x - 1 >= 0) ? m_vecVertices[idx - 1].m_v3Pos : pos;
			SVector3Df up = (z - 1 >= 0) ? m_vecVertices[idx - gridWidth].m_v3Pos : pos;
			SVector3Df down = (z + 1 < gridHeight) ? m_vecVertices[idx + gridWidth].m_v3Pos : pos;

			// Compute vectors to neighbors
			SVector3Df dx = right - left;
			SVector3Df dz = down - up;

			// Cross product to get normal
			SVector3Df normal = dz.cross(dx).normalize();
			normals[idx] = normal;
		}
	}

	// Assign normals back to vertices
	for (size_t i = 0; i < m_vecVertices.size(); i++)
	{
		m_vecVertices[i].m_v3Normals = normals[i]; // Assuming m_vecVertices has a normal field
	}
}

void CGeoMipGrid::SetCurrentTextureIndex(GLint iTexIdx)
{
	m_iCurTextureIndex = iTexIdx;
}

GLint CGeoMipGrid::GetPatchIndexFromWorldPos(const SVector2Df& v2WorldPos) const
{
	GLint iPatchX = static_cast<GLint>(v2WorldPos.x / m_iPatchSize);
	GLint iPatchZ = static_cast<GLint>(v2WorldPos.y / m_iPatchSize);	// note: y here means Z in 2D terrain

	if (iPatchX < 0 || iPatchX >= m_iNumPatchesX || iPatchZ < 0 || iPatchZ >= m_iNumPatchesZ)
		return -1; // outside bounds

	return (iPatchZ * m_iNumPatchesX + iPatchX);
}

/// Splat map Implementation
void CGeoMipGrid::SetupSplatTextures()
{
	const GLint iSplatResolution = 64; // Resolution per patch
	const GLint iNumPatches = m_iNumPatchesX * m_iNumPatchesZ;

	m_vSplatBindings.resize(iNumPatches);
	m_vSplatGLTextures.resize(iNumPatches);
	m_vSplatData.resize(iNumPatches);

	for (GLint i = 0; i < iNumPatches; i++)
	{
		m_vSplatData[i].resize(iSplatResolution * iSplatResolution, SVector4Df(0.0f));

		m_vSplatBindings[i].iTextureIndices[0] = 0;
		m_vSplatBindings[i].iTextureIndices[1] = 1;
		m_vSplatBindings[i].iTextureIndices[2] = 2;
		m_vSplatBindings[i].iTextureIndices[3] = 3;

		m_vSplatGLTextures[i] = new CTexture(GL_TEXTURE_2D);
		m_vSplatGLTextures[i]->GenerateEmptyTexture2D(iSplatResolution, iSplatResolution);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGeoMipGrid::UploadSplatBindings()
{
	if (!m_uiSplatTerrainHandlesSSBO)
	{
		glGenBuffers(1, &m_uiSplatTerrainHandlesSSBO);
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_uiSplatTerrainHandlesSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_vSplatBindings.size() * sizeof(TPatchSplatBinding), m_vSplatBindings.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_uiSplatTerrainHandlesSSBO);
}

void CGeoMipGrid::PaintSplatmap(EBrushType brushType, const TBrushParams& brush)
{
	const GLint iSplatResolution = 64; // Resolution per patch
	const GLfloat fTexelSize = (m_iPatchSize * m_fWorldScale) / iSplatResolution;
	const GLint iPatchIndex = GetPatchIndexFromWorldPos(brush.v2WorldPos);
	const SVector2Df v2WorldPos = brush.v2WorldPos; // Must be in terrain world units (not grid)

	// Calcualte Patch Origin
	GLint iPatchX = iPatchIndex % m_iNumPatchesX;
	GLint iPatchZ = iPatchIndex / m_iNumPatchesX;
	SVector2Df v2PatchOrigin = SVector2Df(iPatchX * m_iPatchSize * m_fWorldScale, iPatchZ * m_iPatchSize * m_fWorldScale);

	auto& vSplatData = m_vSplatData[iPatchIndex];

	for (GLint y = 0; y < iSplatResolution; y++)
	{
		for (GLint x = 0; x < iSplatResolution; x++)
		{
			SVector2Df v2TexelWorld = v2PatchOrigin + SVector2Df(x + 0.5f, y + 0.5f) * fTexelSize;
			float fDist = (v2TexelWorld - v2WorldPos).length();

			if (fDist <= brush.fRadius)
			{
				float fFallOff = 1.0f - (fDist / brush.fRadius);
				fFallOff = MyMath::fclamp(fFallOff, 0.0f, 1.0f);

				GLint iIndex = y * iSplatResolution + x;
				vSplatData[iIndex][brush.iSelectedTexChannel] = MyMath::fclamp(vSplatData[iIndex][brush.iSelectedTexChannel] + brush.fStrength * fFallOff, 0.0f, 1.0f);

				// Normalize to ensure the sum of channels does not exceed 1.0
				float fTotal = vSplatData[iIndex].r + vSplatData[iIndex].g + vSplatData[iIndex].b + vSplatData[iIndex].a;
				if (fTotal > 1.0f)
				{
					// Reduce other channels proportionally
					float overflow = fTotal - 1.0f;
					for (int i = 0; i < 4; i++)
					{
						if (i != brush.iSelectedTexChannel)
						{
							vSplatData[iIndex][i] = std::max(0.0f, vSplatData[iIndex][i] - (overflow * vSplatData[iIndex][i] / fTotal));
						}
					}
					vSplatData[iIndex][brush.iSelectedTexChannel] = 1.0f - (vSplatData[iIndex].r + vSplatData[iIndex].g + vSplatData[iIndex].b);
				}
				else if (fTotal < 0.0f)
				{
					vSplatData[iIndex] = SVector4Df(0.0f);
				}
			}
		}
	}

	UploadSplatmapToGPU(iPatchIndex);
}

void CGeoMipGrid::UploadSplatmapToGPU(GLint iPatchIndex)
{
	const GLint iSplatResolution = 64; // Resolution per patch
	glBindTexture(GL_TEXTURE_2D, m_vSplatGLTextures[iPatchIndex]->GetTextureID());
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, iSplatResolution, iSplatResolution, GL_RGBA, GL_FLOAT, m_vSplatData[iPatchIndex].data());
}
