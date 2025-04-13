#pragma once

#include <glad/glad.h>
#include <vector>

class CBaseTerrain;

class CTriangleList
{
public:
	CTriangleList();
	~CTriangleList();

	void CreateTriangleList(GLint Width, GLint Depth, const CBaseTerrain* pTerrain);
	void Render();

protected:
	typedef struct SVertex
	{
		SVector3Df v3Pos;
		SVector2Df v2TexCoords;
		SVector3Df v3Normals = SVector3Df(0.0f);
		void InitVertex(const CBaseTerrain* pTerrain, GLint x, GLint z);
	} TVertex;

	void CreateGLState();
	void PopulateBuffers(const CBaseTerrain* pTerrain);

	void InitVertices(const CBaseTerrain* pTerrain);
	void InitIndices();
	void CalculateNormals();

	void Destroy();

private:
	GLint m_iWidth;
	GLint m_iDepth;
	GLuint m_iVAO; // Vertex Array Object
	GLuint m_iVBO; // Vertex Buffer Object
	GLuint m_iIdxBuf; // Index Buffer

	std::vector<SVertex> m_vVertices;
	std::vector<GLuint> m_vIndices;
};

class CWaterTriangleList
{
public:
	CWaterTriangleList();
	~CWaterTriangleList();

	void CreateTriangleList(GLint Width, GLint Depth, float fWorldScale);
	void Render();

	void CircleBrush(GLint iX, GLint iZ, GLfloat fSize, GLfloat fRadius, GLfloat fNewHeight = 50.0f);

protected:
	typedef struct SVertex
	{
		SVector3Df v3Pos;
		SVector2Df v2TexCoords;
		SVector3Df v3Normals = SVector3Df(0.0f);
		void InitVertex(GLint width, GLint depth, GLint x, GLint z, float fWorldScale);
	} TVertex;

	void CreateGLState();
	void PopulateBuffers(float fWorldScale);

	void InitVertices(float fWorldScale);
	void InitIndices();

	void CalculateNormals();

	void Destroy();
	void UpdateVertexBuffer();

private:
	GLint m_iWidth;
	GLint m_iDepth;
	GLuint m_iVAO; // Vertex Array Object
	GLuint m_iVBO; // Vertex Buffer Object
	GLuint m_iIdxBuf; // Index Buffer

	std::vector<SVertex> m_vVertices;
	std::vector<GLuint> m_vIndices;
};