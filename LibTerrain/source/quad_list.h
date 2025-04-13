#pragma once

#include <glad/glad.h>
#include "../../LibMath/source/vectors.h"
#include <vector>

enum EBrushType
{
	BRUSH_TYPE_UP,
	BRUSH_TYPE_DOWN,
	BRUSH_TYPE_FLATTEN,
	BRUSH_TYPE_SMOOTH,
	BRUSH_TYPE_NOISE,
};

class CBaseTerrain;

class CQuadList
{
public:
	CQuadList();
	~CQuadList();

	void Destroy() const;

	void CreateQuadList(GLint iWidth, GLint iDepth, const CBaseTerrain* pTerrain);
	void Render() const;
	void CircleBrush(EBrushType eBrushType, GLint iX, GLint iZ, GLfloat fSize, GLfloat fRadius, GLfloat fNewHeight);

	void ApplyTerrainBrush_World(EBrushType eBrushType, float worldX, float worldZ, float fRadius, float fStrength, float fWorldScale);

protected:

	void CreateGLState();
	void PopulateBuffers(const CBaseTerrain* pTerrain);
	void InitVertices(const CBaseTerrain* pTerrain);
	void InitIndices();

	void UpdateVertexBuffer();

public:
	typedef struct SQuadVertex
	{
		SVector3Df m_v3Pos;
		SVector2Df m_v2TexPos;
		void InitQuadVertex(const CBaseTerrain* pTerrain, GLint iWidth, GLint iDepth, GLint iX, GLint iZ);
	} TQuadVertex;

	GLint GetWidth() const;
	GLint GetDepth() const;

	const std::vector<CQuadList::TQuadVertex>& GetVertices() const;
	const std::vector<GLuint>& GetIndices() const;

private:
	GLint m_iWidth;
	GLint m_iDepth;

	GLuint m_uiVAO;
	GLuint m_uiVBO;
	GLuint m_uiIB;

	std::vector<TQuadVertex> m_vecVertices;
	std::vector<GLuint> m_vecIndices;
};