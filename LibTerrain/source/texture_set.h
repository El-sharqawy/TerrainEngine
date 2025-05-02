#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <map>
#include "../../LibGL/source/texture.h"
#include "../../LibMath/source/stdafx.h"
#include <nlohmann/json.hpp> // Requires JSON for Modern C++ library
#include "terrain.h"

using json = nlohmann::json;

enum ETerrainData
{
	TERRAIN_SIZE = 513,
	TERRAIN_PATCH_SIZE = 33,
	TERRAIN_PATCH_COUNT = TERRAIN_SIZE / TERRAIN_PATCH_SIZE,
	TERRAIN_MAX_TEXTURES = 256,
	CELL_SIZE = 1,
};

typedef struct STerrainTexture
{
	std::string m_stFileName;
	GLuint m_uiTextureID;
	CTexture* m_pTexture;
	GLfloat m_fUScale;
	GLfloat m_fVScale;
	GLfloat m_fUOffset;
	GLfloat m_fVOffset;
	bool m_bIsSplat;
	GLuint m_uiHeightMin;
	GLuint m_uiHeightMax;
	CMatrix4Df m_matTransform;

	STerrainTexture()
	{
		m_stFileName.clear();
		m_uiTextureID = 0;
		m_pTexture = nullptr;
		m_fUScale = 4.0f;
		m_fVScale = 4.0f;
		m_fUOffset = 0.0f;
		m_fVOffset = 0.0f;
		m_bIsSplat = true;
		m_uiHeightMin = 0;
		m_uiHeightMax = 65535; // Check Later
		m_matTransform.InitIdentity();
	}

	json ToJson() const
	{
		return {
			{"filename", m_stFileName},
			{"u_scale", m_fUScale},
			{"v_scale", m_fVScale},
			{"u_offset", m_fUOffset},
			{"v_offset", m_fVOffset},
			{"is_splat", m_bIsSplat},
			{"begin", m_uiHeightMin},
			{"end", m_uiHeightMax},
		};
	}

} TTerrainTexture;

typedef struct STerrainMaterial
{
	std::string m_stName;
	SVector3Df m_v3Ambient;		/* Ambient Color */
	SVector3Df m_v3Diffuse;		/* Diffuse Color */
	SVector3Df m_v3Specular;	/* Specular Color */
	GLfloat m_fSpec_Power;		/* Specular Power */
} TTerrainMaterial;

typedef struct STerrainSplat
{
	// OpenGL texture handle
	CTexture* m_pTexture;

	// Whether texture needs regeneration
	bool m_bUpdate;

	// Whether this texture is in use
	bool m_bIsActive;
} TTerrainSplat;

typedef struct STerrainSplatPatch
{
	// Count of tiles using each texture across entire terrain
	GLint m_iTileCount[TERRAIN_MAX_TEXTURES];
	
	// Tile counts per patch per texture [patchX][patchZ][textureIndex]
	GLint m_iPatchTileCount[TERRAIN_PATCH_COUNT][TERRAIN_PATCH_COUNT][TERRAIN_MAX_TEXTURES];

	// Array of splat textures
	TTerrainSplat m_Splats[TERRAIN_MAX_TEXTURES];

	// Whether patch needs GPU update
	bool m_bNeedUpdate;
} TTerrainSplatPatch;

class CTerrainTextureSet
{
public:
	typedef std::vector<TTerrainTexture> TTexturesVector;

	CTerrainTextureSet();
	~CTerrainTextureSet();

	void Clear();
	void Create();

	size_t GetTexturesCount();
	TTerrainTexture& GetTexture(size_t iIndex);
	bool RemoveTexture(size_t iIndex);

	bool Save(const std::string& stFileName);
	bool Load(const std::string& stFileName);

	void Reload();

	bool SetTexture(size_t iIndex, const TTerrainTexture& Texture);

	bool SetTexture(size_t iIndex,
		const std::string& stFileName,
		GLfloat fUScale,
		GLfloat fVScale,
		GLfloat fUOffset,
		GLfloat fVOffset,
		bool bIsSplat,
		GLuint uiHeightMin,
		GLuint uiHeightMax);

	bool AddTexture(const TTerrainTexture& Texture);

	bool AddTexture(const std::string& stFileName,
		GLfloat fUScale,
		GLfloat fVScale,
		GLfloat fUOffset,
		GLfloat fVOffset,
		bool bIsSplat,
		GLuint uiHeightMin,
		GLuint uiHeightMax);

	bool IsValidImage(const std::string& filePath);

	void ResizeTextures(size_t iNum);
	void IncreaseTexturesNum(size_t iNum);

	TTexturesVector& GetTextures();

	CTexture* GetEraserTexture();

protected:
	void AddEmptyTexture();

private:
	TTexturesVector m_vTextures;
	TTerrainTexture m_tErrorTexture;
	std::string m_stFileName;
	GLfloat m_fTerrainTexCoordBase;
};