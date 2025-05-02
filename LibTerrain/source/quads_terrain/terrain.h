#pragma once

#include "quad_list.h"
#include "object.h"
#include "simple_water.h"

class CBaseTerrain : public CObject
{
public:
	CBaseTerrain();
	~CBaseTerrain();

	void Destroy();

	bool InitializeTerrain(const float fWorldScale, const float fTextureScale, const std::vector<std::string>& vTexturesNames);
	void InitializeShaders();

	void LoadFromFile(const std::string& sFileName);
	void SaveToFile(const std::string& sFileName);

	void SetTexture(CTexture* pTexture, GLint iIndex);
	void SetMinHeight(const float fVal);
	void SetMaxHeight(const float fVal);
	void SetMinMaxHeight(const float fMinVal, const float fMaxVal);
	void SetTexturesHeights(const float fTexHeight0, const float fTexHeight1, const float fTexHeight2, const float fTexHeight3);
	void SetLightDir(const SVector3Df& v3LightDir);

	float GetHeight(GLint iX, GLint iZ) const;
	float GetHeightInterpolated(GLfloat fX, GLfloat fZ) const;
	float GetWorldScale() const;
	float GetWorldSize() const;
	float GetWorldHeight(GLfloat fX, GLfloat fZ) const;
	float GetTextureScale() const;
	float GetMinHeight() const;
	float GetMaxHeight() const;
	GLint GetSize() const;
	SVector3Df ConstrainCameraToTerrain();

	void ApplyTerrainBrush_World(EBrushType eBrushType, GLfloat worldX, GLfloat worldZ, GLfloat fRadius, GLfloat fStrength);

	CQuadList& GetQuadList()
	{
		return m_gQuadList;
	}

	GLint GetWidth() const;
	GLint GetDepth() const;

	float GetWorldWidth() const;
	float GetWorldDepth() const;

	const std::vector<CQuadList::TQuadVertex>& GetVertices() const;
	const std::vector<GLuint>& GetIndices() const;

	GLint GetPatchSize() const;

	float GetRoughness() const;
	void SetRoughness(const float fRoughness);

	virtual void SetGUI() {}

	//if the class will cointain some logic, so it must be refreshed at each game loop cycle by calling update. Otherwise just don't override it.  
	virtual void Update() {}

	void Render(const CCamera& rCamera);
	void RenderTerrain(const CCamera& rCamera);
	//void RenderTerrainReflectionPass(const CCamera& rCamera);
	//void RenderTerrainRefractionPass(const CCamera& rCamera);
	//void RenderTerrainDefaultPass(const CCamera& rCamera);
	//void RenderWater(const CCamera& rCamera);

	CShader* GetShader() const;
	CShader& GetShaderRef() const;

	CTexture GetHeightMapTex() const;
	const CGrid<float>& GetGridMap() const;

protected:
	bool LoadHeightMapFile(const std::string& sFileName);
	void Finalize();

protected:
	GLint m_iTerrainSize;
	GLint m_iNumPatches;
	float m_fWorldScale;
	CGrid<float> m_fHeightMapGrid;
	float m_fMinHeight;
	float m_fMaxHeight;
	CShader* m_pTerrainShader;
	float m_fRoughness;

private:
	float m_fTextureScale;
	CTexture* m_pTextures[4];
	CTexture m_gHeightMapTex;
	CQuadList m_gQuadList;

	SVector3Df m_v3LightDir;
	float m_fCameraHeight;
	//CSimpleWater m_gSimpleWater;
};