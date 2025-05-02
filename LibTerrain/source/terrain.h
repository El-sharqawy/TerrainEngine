#pragma once

#include <glad/glad.h>
#include "../../LibMath/source/grid.h"
#include "../../LibGL/source/shader.h"
#include "geomip_grid.h"
#include "object.h"
#include "texture_set.h"
#include "../../LibImageUI/imgui.h"

class CBaseTerrain : public CSingleton<CBaseTerrain>, public CObject
{
public:
	CBaseTerrain();
	~CBaseTerrain();

	void InitializeTerrain(GLint iTerrainSize, GLint iPatchSize, GLfloat fWorldScale, GLfloat fTextureScale);

	GLint GetSize() const;
	GLint GetPatchSize() const;
	GLint GetWidth() const;
	GLint GetDepth() const;
	GLfloat GetWorldScale() const;
	GLfloat GetTextureScale() const;
	GLfloat GetHeight(GLint iX, GLint iZ) const;

	CGrid<GLfloat>* GetMapGrid();
	CShader* GetTerrainShader();
	CGeoMipGrid* GetGeoMipGrid();
	CWorldTranslation* GetWorldTranslation();

	std::vector<CGeoMipGrid::TVertex>& GetVertices() const;
	std::vector<GLuint>& GetIndices() const;
	std::vector<TLodInfo>& GetLodInfo() const;

	void UpdateVertexBuffer();

	float GetHeightInterpolated(GLfloat fX, GLfloat fZ) const;
	float GetWorldSize() const;
	float GetWorldHeight(GLfloat fX, GLfloat fZ) const;

	SVector3Df ConstrainCameraToTerrain();

	virtual void Render();
	virtual void SetGUI();
	virtual void Update();

	void SetLightDirection(const SVector3Df& v3LightDir);

	const TTerrainTexture& GetTerrainTexture(size_t iIndex) const;
	void AddTerrainTexture(const TTerrainTexture& terrainTex) const;

	size_t GetTexturesCount() const;
	void SetCurrentTexture(size_t iIndex);

	void LoadTextureSet(const std::string& stFileName);

	void DoBindlesslyTexturesSetup();

private:
	void InitializeShaders();

public:
	static void SetTextureSet(CTerrainTextureSet* pTextureSet);
	static CTerrainTextureSet* GetTextureSet();

protected:	// allow it to be accessed through inheritance
	GLint m_iTerrainSize;
	GLint m_iPatchSize;
	CGrid<GLfloat>* m_pMapGrid;
	GLfloat m_fWorldScale;
	GLfloat m_fTextureScale;
	std::vector<CTexture*> m_vSpatTextures;

private:
	CShader* m_pTerrainShader;
	CGeoMipGrid* m_pGeoMapGrid;
	SVector3Df m_v3LightDir;
	CWorldTranslation* m_pWorldTranslation;
	
	GLint m_iSelectedBtnIdx;

	static CTerrainTextureSet* ms_pTerrainTextureSet;

	GLuint m_uiTerrainHandlesSSBO; // for Bindless textures
	std::vector<GLuint64> m_vTextureHandles;

};

