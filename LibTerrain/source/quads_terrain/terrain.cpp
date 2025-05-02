#include "stdafx.h"
#include "terrain.h"

CBaseTerrain::CBaseTerrain() : m_fHeightMapGrid()
{
	m_iTerrainSize = 0;
	m_iNumPatches = 0;
	m_fWorldScale = 1.0f;
	m_fTextureScale = 1.0f;
	arr_mem_zero(m_pTextures);

	m_fMinHeight = 0.0f;
	m_fMaxHeight = 0.0f;
	m_pTerrainShader = new CShader("TerrainShader");
	m_v3LightDir = SVector3Df(1.0f);
	m_fCameraHeight = 50.0f;
	m_fRoughness = 1.0f;
}

CBaseTerrain::~CBaseTerrain()
{
	delete m_pTerrainShader;
	m_pTerrainShader = nullptr;

	Destroy();
}

void CBaseTerrain::Destroy()
{
	m_fHeightMapGrid.Destroy();
	m_gQuadList.Destroy();
}

bool CBaseTerrain::InitializeTerrain(const float fWorldScale, const float fTextureScale, const std::vector<std::string>& vTexturesNames)
{
	InitializeShaders();

	if (vTexturesNames.size() != arr_size(m_pTextures))
	{
		sys_err("Failed to Initialize Terrains Textures given textures(%zu) not equal to the actual array (%zu)", vTexturesNames.size(), arr_size(vTexturesNames));
		return false;
	}

	m_fWorldScale = fWorldScale;
	m_fTextureScale = fTextureScale;

	for (GLint i = 0; i < arr_size(m_pTextures); i++)
	{
		m_pTextures[i] = new CTexture();
		m_pTextures[i]->Load(vTexturesNames[i]);
	}

	return true;
}

void CBaseTerrain::InitializeShaders()
{
	m_pTerrainShader->AttachShader("shaders/quads_terrain_shader/terrain.vert");
	m_pTerrainShader->AttachShader("shaders/quads_terrain_shader/terrain.tcs");
	m_pTerrainShader->AttachShader("shaders/quads_terrain_shader/terrain.tes");
	m_pTerrainShader->AttachShader("shaders/quads_terrain_shader/terrain.frag");
	m_pTerrainShader->LinkPrograms();

	m_pTerrainShader->Use();
	m_pTerrainShader->setInt("TextureHeight0", COLOR_TEXTURE_UNIT_INDEX_0);
	m_pTerrainShader->setInt("TextureHeight1", COLOR_TEXTURE_UNIT_INDEX_1);
	m_pTerrainShader->setInt("TextureHeight2", COLOR_TEXTURE_UNIT_INDEX_2);
	m_pTerrainShader->setInt("TextureHeight3", COLOR_TEXTURE_UNIT_INDEX_3);
	m_pTerrainShader->setInt("HeightMapTex", HEIGHT_MAP_TEXTURE_UNIT_INDEX);
}

void CBaseTerrain::LoadFromFile(const std::string& sFileName)
{
	if (m_iNumPatches == 0)
	{
		sys_err("CBaseTerrain::LoadFromFile: cannot Load Height Map File %s, No Patches Numbers!", sFileName.c_str());
		return;
	}

	LoadHeightMapFile(sFileName);
	m_gQuadList.CreateQuadList(m_iNumPatches, m_iNumPatches, this);
}

void CBaseTerrain::SaveToFile(const std::string& sFileName)
{
	unsigned char* ptr = (unsigned char*)malloc(m_iTerrainSize * m_iTerrainSize);

	if (!ptr)
	{
		sys_err("CBaseTerrain::SaveToFile: %s Failed To Allocate %u bytes using Malloc", sFileName.c_str(), m_iTerrainSize * m_iTerrainSize);
		return;
	}

	float* pSrc = m_fHeightMapGrid.GetBaseAddr();

	float fDelta = m_fMaxHeight - m_fMinHeight;

	for (GLint i = 0; i < m_iTerrainSize * m_iTerrainSize; i++)
	{
		float f = (pSrc[i] - m_fMinHeight) / fDelta;
		ptr[i] = (unsigned char)(f * 255.0f);
	}

	stbi_write_png(sFileName.c_str(), m_iTerrainSize, m_iTerrainSize, 1, ptr, m_iTerrainSize);
	safe_free(ptr);
}

void CBaseTerrain::SetTexture(CTexture* pTexture, GLint iIndex)
{
	if (iIndex >= arr_size(m_pTextures))
	{
		sys_err("Cannot Set Textures, Index %d out of range!", iIndex);
		return;
	}

	m_pTextures[iIndex] = pTexture;
}

void CBaseTerrain::SetMinHeight(const float fVal)
{
	m_fMinHeight = fVal;
}

void CBaseTerrain::SetMaxHeight(const float fVal)
{
	m_fMaxHeight = fVal;
}

void CBaseTerrain::SetMinMaxHeight(const float fMinVal, const float fMaxVal)
{
	m_fMinHeight = fMinVal;
	m_fMaxHeight = fMaxVal;
}

void CBaseTerrain::SetTexturesHeights(const float fTexHeight0, const float fTexHeight1, const float fTexHeight2, const float fTexHeight3)
{
	m_pTerrainShader->Use();
	m_pTerrainShader->setFloat("fTexHeight0", fTexHeight0);
	m_pTerrainShader->setFloat("fTexHeight1", fTexHeight1);
	m_pTerrainShader->setFloat("fTexHeight2", fTexHeight2);
	m_pTerrainShader->setFloat("fTexHeight3", fTexHeight3);

}

void CBaseTerrain::SetLightDir(const SVector3Df& v3LightDir)
{
	m_v3LightDir = v3LightDir;
	m_pTerrainShader->Use();
	SVector3Df ReversedLightDir = m_v3LightDir * -1.0f;
	ReversedLightDir = ReversedLightDir.normalize();
	m_pTerrainShader->setVec3("v3LightDirection", ReversedLightDir);
}

float CBaseTerrain::GetHeight(GLint iX, GLint iZ) const
{
	return (m_fHeightMapGrid.Get(iX, iZ));
}

float CBaseTerrain::GetHeightInterpolated(GLfloat fX, GLfloat fZ) const
{
	const float X0Z0Height = GetHeight(cast_intf(fX), cast_intf(fZ));

	if (cast_intf(fX) + 1 >= m_iTerrainSize || cast_intf(fZ) + 1 >= m_iTerrainSize)
	{
		return (X0Z0Height);
	}

	const float X1Z0Height = GetHeight(cast_intf(fX) + 1, cast_intf(fZ));
	const float X0Z1Height = GetHeight(cast_intf(fX), cast_intf(fZ) + 1);
	const float X1Z1Height = GetHeight(cast_intf(fX) + 1, cast_intf(fZ) + 1);

	const float fFactorX = fX - std::floorf(fX);

	const float fInterpolatedBottom = (X1Z0Height - X0Z0Height) * fFactorX + X0Z0Height;
	const float fInterpolatedTop = (X1Z1Height - X0Z1Height) * fFactorX + X0Z1Height;

	const float fFactorZ = fZ - std::floor(fZ);

	const float fFinalHeight = (fInterpolatedTop - fInterpolatedBottom) * fFactorZ + fInterpolatedBottom;

	return (fFinalHeight);
}

float CBaseTerrain::GetWorldScale() const
{
	return (m_fWorldScale);
}

float CBaseTerrain::GetWorldSize() const
{
	return (cast_floati(m_iNumPatches) * m_fWorldScale);
}

/* Get Height by X - Z In World */
float CBaseTerrain::GetWorldHeight(GLfloat fX, GLfloat fZ) const
{
	const float fHeightMapX = fX / GetWorldScale();
	const float fHeightMapZ = fZ / GetWorldScale();

	return (GetHeightInterpolated(fHeightMapX, fHeightMapZ));
}

float CBaseTerrain::GetTextureScale() const
{
	return (m_fTextureScale);
}

float CBaseTerrain::GetMinHeight() const
{
	return (m_fMinHeight);
}

float CBaseTerrain::GetMaxHeight() const
{
	return (m_fMaxHeight);
}

GLint CBaseTerrain::GetSize() const
{
	return (m_iTerrainSize);
}

SVector3Df CBaseTerrain::ConstrainCameraToTerrain()
{
	SVector3Df v3CamPos = CCameraManager::Instance().GetCurrentCamera()->GetPosition();
	SVector3Df v3NewCamPos = v3CamPos;

	// Make sure camera doesn't go outside of the terrain bounds
	if (v3CamPos.x < 0.0f)
	{
		v3NewCamPos.x = 0.0f;
	}

	if (v3CamPos.z < 0.0f)
	{
		v3NewCamPos.z = 0.0f;
	}
	if (v3CamPos.x >= GetWorldSize())
	{
		v3NewCamPos.x = GetWorldSize() - 0.5f;
	}

	if (v3CamPos.z >= GetWorldSize())
	{
		v3NewCamPos.z = GetWorldSize() - 0.5f;
	}

	v3NewCamPos.y = GetWorldHeight(v3CamPos.x, v3CamPos.z) + m_fCameraHeight;
	float fSmoothHeight = std::sinf(v3CamPos.x * 4.0f) + std::cosf(v3CamPos.z * 4.0f);
	fSmoothHeight /= 35.0f;

	v3NewCamPos.y += fSmoothHeight;

	CCameraManager::Instance().GetCurrentCamera()->SetPosition(v3NewCamPos);

	return (v3NewCamPos);
}

bool CBaseTerrain::LoadHeightMapFile(const std::string& sFileName)
{
	GLint iFileSize = 0;
	unsigned char* p = (unsigned char*)ReadBinaryFile(sFileName.c_str(), iFileSize);

	if (iFileSize % sizeof(float) != 0)
	{
		sys_err("CBaseTerrain::LoadHeightMapFile: %s does not contain an whole number of floats (size %d)", sFileName.c_str(), iFileSize);
		return false;
	}

	m_iTerrainSize = static_cast<GLint>(std::sqrtf(static_cast<float>(iFileSize) / static_cast<float>(sizeof(float))));

	sys_log("CBaseTerrain::LoadHeightMapFile: Terrain Size: %d", m_iTerrainSize);

	if ((m_iTerrainSize * m_iTerrainSize) != (iFileSize % sizeof(float)))
	{
		sys_err("CBaseTerrain::LoadHeightMapFile: %s does not contain a square height map - size %d", sFileName.c_str(), iFileSize);
		return false;
	}

	m_fHeightMapGrid.InitGrid(m_iTerrainSize, m_iTerrainSize, (float*)p);

	return (true);
}

void CBaseTerrain::Finalize()
{
	m_gQuadList.CreateQuadList(m_iNumPatches, m_iNumPatches, this);
	m_gHeightMapTex.LoadF32(m_iTerrainSize, m_iTerrainSize, m_fHeightMapGrid.GetBaseAddr());
	m_gQuadList.ApplyHeightmap(m_fHeightMapGrid.GetBaseAddr(), m_iTerrainSize, m_iTerrainSize);

	//m_gSimpleWater.Init(m_iTerrainSize, m_fWorldScale);
}

void CBaseTerrain::Render(const CCamera& rCamera)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	RenderTerrain(rCamera);
	//RenderWater(rCamera);
}

void CBaseTerrain::RenderTerrain(const CCamera& rCamera)
{
	CMatrix4Df matVewProj = rCamera.GetViewProjMatrix();
	CMatrix4Df matView = rCamera.GetMatrix();

	m_pTerrainShader->Use();
	m_pTerrainShader->setMat4("mat4ViewProjection", matVewProj);
	m_pTerrainShader->setMat4("mat4View", matView);

	for (GLint i = 0; i < arr_size(m_pTextures); i++)
	{
		if (m_pTextures[i])
		{
			m_pTextures[i]->Bind(COLOR_TEXTURE_UNIT_0 + i);
		}
	}

	m_gHeightMapTex.Bind(HEIGHT_MAP_TEXTURE_UNIT);

	SVector3Df ReversedLightDir = m_v3LightDir * -1.0f;
	ReversedLightDir = ReversedLightDir.normalize();

	SSceneElements* scene = CObject::pScene;
	m_v3LightDir = scene->v3LightDir;

	m_pTerrainShader->setVec3("v3LightDir", scene->v3LightDir);
	m_pTerrainShader->setVec3("v3LightColor", scene->v3LightColor);
	glFrontFace(GL_CCW);
	m_gQuadList.Render();
	glFrontFace(GL_CW);

	//RenderTerrainReflectionPass(rCamera);
	//RenderTerrainRefractionPass(rCamera);
	//RenderTerrainDefaultPass(rCamera);
}

/*
void CBaseTerrain::RenderTerrainReflectionPass(const CCamera& rCamera)
{
	m_gSimpleWater.StartReflectionPass();

	CCamera UnderWaterCam = rCamera;

	// Set the position of the camera to be under the water
	const SVector3Df& v3CamPos = rCamera.GetPosition();
	float fCamHeightAboveWater = v3CamPos.y - m_gSimpleWater.GetWaterHeight();
	SVector3Df v3CamPosUnderWater = v3CamPos;
	v3CamPosUnderWater.y = m_gSimpleWater.GetWaterHeight() - fCamHeightAboveWater;

	UnderWaterCam.SetPosition(v3CamPosUnderWater);

	// Flip the target vector of the under water camera over the Y axis
	SVector3Df v3CamTargetUnderWater = rCamera.GetTarget();
	v3CamTargetUnderWater.y *= -1.0f;
	UnderWaterCam.SetTarget(v3CamTargetUnderWater);

	SVector3Df v3PlaneNormal(0.0f, 1.0f, 0.0f);
	SVector3Df v3PointOnPlane(0.0f, m_gSimpleWater.GetWaterHeight() + 0.5f, 0.0f);

	float fDot = -v3PlaneNormal.dot(v3PointOnPlane);
	m_pTerrainShader->setVec4("v4ClipPlane", v3PlaneNormal.x, v3PlaneNormal.y, v3PlaneNormal.z, fDot);
	m_gSimpleWater.StopReflectionPass();
}

void CBaseTerrain::RenderTerrainRefractionPass(const CCamera& rCamera)
{
	m_gSimpleWater.StartRefractionPass();

	SVector3Df v3PlaneNormal(0.0f, 1.0f, 0.0f);
	SVector3Df v3PointOnPlane(0.0f, m_gSimpleWater.GetWaterHeight() + 0.5f, 0.0f);

	float fDot = -v3PlaneNormal.dot(v3PointOnPlane);
	m_pTerrainShader->setVec4("v4ClipPlane", v3PlaneNormal.x, v3PlaneNormal.y, v3PlaneNormal.z, fDot);
	glFrontFace(GL_CCW);
	m_gQuadList.Render();
	glFrontFace(GL_CW);

	m_gSimpleWater.StopRefractionPass();
}

void CBaseTerrain::RenderTerrainDefaultPass(const CCamera& rCamera)
{
	SVector3Df v3PlaneNormal(0.0f, 1.0f, 0.0f);
	SVector3Df v3PointOnPlane(0.0f, 0.0f, 0.0f);
	float fDot = -v3PlaneNormal.dot(v3PointOnPlane);
	m_pTerrainShader->setVec4("v4ClipPlane", v3PlaneNormal.x, v3PlaneNormal.y, v3PlaneNormal.z, fDot);
	glFrontFace(GL_CCW);
	m_gQuadList.Render();
	glFrontFace(GL_CW);
}

void CBaseTerrain::RenderWater(const CCamera& rCamera)
{
	m_gSimpleWater.Render(rCamera.GetPosition(), rCamera.GetViewProjMatrix(), m_v3LightDir);
}
*/

GLint CBaseTerrain::GetWidth() const
{
	return (m_gQuadList.GetWidth());
}

GLint CBaseTerrain::GetDepth() const
{
	return (m_gQuadList.GetDepth());
}

const std::vector<CQuadList::TQuadVertex>& CBaseTerrain::GetVertices() const
{
	return (m_gQuadList.GetVertices());
}

const std::vector<GLuint>& CBaseTerrain::GetIndices() const
{
	return (m_gQuadList.GetIndices());
}

float CBaseTerrain::GetWorldWidth() const
{
	return (static_cast<float>(m_gQuadList.GetWidth()) * m_fWorldScale);
}

float CBaseTerrain::GetWorldDepth() const
{
	return (static_cast<float>(m_gQuadList.GetDepth()) * m_fWorldScale);
}

GLint CBaseTerrain::GetPatchSize() const
{
	return (m_iNumPatches);
}

void CBaseTerrain::ApplyTerrainBrush_World(EBrushType eBrushType, GLfloat worldX, GLfloat worldZ, GLfloat fRadius, GLfloat fStrength)
{
	float fWorldScale = GetWorldScale();
	m_gQuadList.ApplyTerrainBrush_World(eBrushType, worldX, worldZ, fRadius, fStrength, fWorldScale);
}

float CBaseTerrain::GetRoughness() const
{
	return (m_fRoughness);
}

void CBaseTerrain::SetRoughness(const float fRoughness)
{
	m_fRoughness = fRoughness;
}

CShader* CBaseTerrain::GetShader() const
{
	return (m_pTerrainShader);
}

CShader& CBaseTerrain::GetShaderRef() const
{
	return (*m_pTerrainShader);
}

CTexture CBaseTerrain::GetHeightMapTex() const
{
	return (m_gHeightMapTex);
}

const CGrid<float>& CBaseTerrain::GetGridMap() const
{
	return (m_fHeightMapGrid);
}
