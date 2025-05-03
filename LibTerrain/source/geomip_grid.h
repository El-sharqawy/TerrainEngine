#pragma once

#include <glad/glad.h>
#include <vector>
#include "lod_manager.h"

class CBaseTerrain;

enum EBrushType
{
	BRUSH_TYPE_NONE,
	BRUSH_TYPE_UP,
	BRUSH_TYPE_DOWN,
	BRUSH_TYPE_FLATTEN,
	BRUSH_TYPE_SMOOTH,
	BRUSH_TYPE_NOISE,
	BRUSH_TYPE_TEXTURE,
	BRUSH_TYPE_ERASER,
};

typedef struct SPatchSplatBinding
{
	GLint iTextureIndices[4]; // Indices into the terrain texture set

	SPatchSplatBinding()
	{
		for (GLint i = 0; i < 4; ++i)
		{
			iTextureIndices[i] = -1;
		}
	}
} TPatchSplatBinding;

struct TSplatLayer
{
	GLubyte bTextureIndex;
	GLfloat fWeight;
};

typedef struct SBrushParams
{
	SVector2Df v2WorldPos;			// Brush center (X, Z)
	GLfloat fRadius;				// Brush radius
	GLfloat fStrength;				// Paint intensity [0-1]
	GLint iSelectedTextureIndex;	// 0=R, 1=G, 2=B, 3=A
	GLfloat fAlpha = 1.0f; // Add this
} TBrushParams;

class CGeoMipGrid
{
public:
	CGeoMipGrid();
	~CGeoMipGrid();

	void CreateGeoMipGrid(GLint iWidth, GLint iDepth, GLint iPatchSize, CBaseTerrain* pTerrain);
	void Render();
	void Render(const SVector3Df& CameraPos, const CMatrix4Df& ViewProj);

	bool IsPatchInsideViewFrustumViewSpace(GLint iX, GLint iZ, const CMatrix4Df& matViewProj) const;
	bool IsPatchInsideViewFrustumWorldSpace(GLint iX, GLint iZ, const SFrustumCulling& sFrustumCulling) const;

	void Destroy();

	void CircleBrush(GLint iX, GLint iZ, GLfloat fSize, GLfloat fRadius, GLfloat fNewHeight = 50.0f);

	typedef struct SVertex
	{
		SVector3Df m_v3Pos;
		SVector2Df m_v2TexCoords;
		SVector3Df m_v3Normals;
		void InitVertex(CBaseTerrain* pTerrain, GLint x, GLint z);
	} TVertex;

	std::vector<CGeoMipGrid::TVertex>& GetVertices();
	std::vector<GLuint>& GetIndices();
	std::vector<TLodInfo>& GetLodInfo();

	GLint GetWidth() const;
	GLint GetDepth() const;
	GLint GetPatchSize() const;
	GLint GetMaxLOD() const;
	GLint GetNumPatchesX() const;
	GLint GetNumPatchesZ() const;
	GLint GetCurrentTextureIndex() const;
	GLuint GetCurrentTexture() const;

	void ApplyTerrainBrush_World(EBrushType eBrushType, GLfloat worldX, GLfloat worldZ, GLfloat fRadius, GLfloat fStrength);
	void UpdateNormals();
	void UpdateVertexBuffer();
	void SetCurrentTextureIndex(GLint iTexIdx);

	GLint GetPatchIndexFromWorldPos(const SVector2Df& v3WorldPos) const;

protected:

	void CreateGLState();
	void PopulateBuffers(CBaseTerrain* pTerrain);

	GLint CalculateNumIndices() const;

	void InitVertices(CBaseTerrain* pTerrain);
	GLint InitIndices();
	GLint InitIndicesLOD(GLint Index, std::vector<GLuint>& Indices, GLint iLOD);
	GLint InitIndicesLODSingle(GLint Index, std::vector<GLuint>& Indices, GLint iCoreLOD, GLint iLeftLOD, GLint iRightLOD, GLint iTopLOD, GLint iBottomLOD);
	GLuint CreateTriangleFan(GLint Index, std::vector<GLuint>& Indices, GLint iCoreLOD, GLint iLeftLOD, GLint iRightLOD, GLint iTopLOD, GLint iBottomLOD, GLint iX, GLint iZ);
	void CalculateNormals();
	GLuint AddTriangle(GLuint uiIndex, std::vector<GLuint>& vIndicies, GLuint v1, GLuint v2, GLuint v3);

private:
	GLint m_iWidth;
	GLint m_iDepth;
	GLint m_iPatchSize;
	GLuint m_uiVAO; // Vertex Array Object
	GLuint m_uiVBO; // Vertex Buffer Object
	GLuint m_uiIdxBuf; // Index Buffer
	GLint m_iCurTextureIndex;
	GLint m_iSplatTexResolution;

	std::vector<TLodInfo> m_vLodInfo;
	GLint m_iMaxLOD;
	GLint m_iNumPatchesX;
	GLint m_iNumPatchesZ;
	CBaseTerrain* m_pTerrain;
	float m_fWorldScale;

	std::vector<SVertex> m_vecVertices;
	std::vector<GLuint> m_vecIndices;

// SplatData Implementation
public:
	struct TSplatData
	{
		std::vector<glm::uvec4> indexData;  // Texture indices (per texel)
		std::vector<glm::vec4> weightData;   // Blending weights (per texel)
	};

	void SetupSplatTextures();
	void UploadSplatBindings();
	void PaintSplatmap(EBrushType eBrushType, const TBrushParams& brush);
	void UploadSplatmapToGPU(GLint iPatchIndex);
	void PaintSplatmap(const TBrushParams& brush);
	float SmoothBrushFalloff(float dist, float radius, float hardness = 0.8f);
	glm::vec2 GetPatchOrigin(int patchIndex) const;
	void ResetAllSplatmapsToBaseTexture();
	void PaintBrushOnSinglePatch(const TBrushParams& brush, int iPatchIndex);
	glm::ivec2 GetPatchIndexFromWorldPos2D(glm::vec2 worldPos);
	int GetPatchLinearIndex(int px, int py);
	bool BrushIntersectsPatch(int patchIndex, const TBrushParams& brush);

private:
	std::vector<CTexture*> m_vIndexMaps;    // GL_RGBA32UI textures
	std::vector<CTexture*> m_vWeightMaps;   // GL_RGBA32F textures
	std::vector<TSplatData> m_vSplatData;   // CPU-side data
	GLuint m_uiSplatIndexHandlesSSBO;		// SSBO for texture handles
	GLuint m_uiSplatWeightHandlesSSBO;		// SSBO for texture handles

};