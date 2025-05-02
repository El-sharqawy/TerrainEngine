	#pragma once

	#include <glad/glad.h>
	#include "../../LibMath/source/vectors.h"
	#include <vector>

	enum EBrushType
	{
		BRUSH_TYPE_NONE,
		BRUSH_TYPE_UP,
		BRUSH_TYPE_DOWN,
		BRUSH_TYPE_FLATTEN,
		BRUSH_TYPE_SMOOTH,
		BRUSH_TYPE_NOISE,
		BRUSH_TYPE_TEXTURE,
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

		bool GetRayTerrainIntersection(const SVector3Df& rayOrigin, const SVector3Df& rayDir, SVector3Df& intersectionPoint);

		void ApplyTerrainBrush_World(EBrushType eBrushType, float worldX, float worldZ, float fRadius, float fStrength, float fWorldScale);

		void ApplyTextureBrush(float worldX, float worldZ, float radius, int textureLayer, float strength, float fWorldScale, bool additive);

		void ApplyHeightmap(const GLfloat* pHeightMap, GLint mapWidth, GLint mapHeight);

	protected:

		void CreateGLState();
		void PopulateBuffers(const CBaseTerrain* pTerrain);
		void InitVertices(const CBaseTerrain* pTerrain);
		void InitIndices();
		void CalculateNormals();

		void UpdateVertexBuffer();

	public:
		typedef struct SQuadVertex
		{
			SVector3Df m_v3Pos;
			SVector3Df m_v3Normals;
			SVector2Df m_v2TexPos;
			SVector4Df m_v4BlendWeights; // New: Blend weights for 4 texture layers
			void InitQuadVertex(const CBaseTerrain* pTerrain, GLint iWidth, GLint iDepth, GLint iX, GLint iZ);
			SVector3Df CalculateNormal(int x, int z, const float* heightmap, int width, int height);
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
		const CBaseTerrain* m_pTerrain;

		std::vector<uint8_t> m_BlendMapData; // RGBA8
	};