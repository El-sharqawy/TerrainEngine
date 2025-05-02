#include "stdafx.h"
#include "terrain.h"
#include "../../LibImageUI/imgui.h"
#include "../../LibImageUI/ImGuiFileDialog.h"
#include "../../LibImageUI/ImGuiFileDialogConfig.h"

CTerrainTextureSet* CBaseTerrain::ms_pTerrainTextureSet = nullptr;

CBaseTerrain::CBaseTerrain()
{
	m_pMapGrid = new CGrid<float>();
	m_pGeoMapGrid = new CGeoMipGrid();
	m_pTerrainShader = new CShader("TerrainShader");
	m_pWorldTranslation = new CWorldTranslation();

	m_iTerrainSize = 32;
	m_iPatchSize = 1;
	m_fWorldScale = 1.0f;
	m_fTextureScale = 1.0f;

	m_v3LightDir = SVector3Df(0.0f, 1.0f, 1.0f);

	m_iSelectedBtnIdx = 0;

	InitializeShaders();
}

CBaseTerrain::~CBaseTerrain()
{
	safe_delete(m_pMapGrid);
	safe_delete(m_pGeoMapGrid);
	safe_delete(m_pTerrainShader);
	safe_delete(m_pWorldTranslation);

	if (m_uiTerrainHandlesSSBO)
	{
		glDeleteBuffers(1, &m_uiTerrainHandlesSSBO);
	}
}

void CBaseTerrain::InitializeTerrain(GLint iTerrainSize, GLint iPatchSize, GLfloat fWorldScale, GLfloat fTextureScale)
{
	m_iTerrainSize = iTerrainSize;
	m_iPatchSize = iPatchSize;
	m_fWorldScale = fWorldScale;
	m_fTextureScale = fTextureScale;

	m_pWorldTranslation->SetScale(fWorldScale);

	m_pMapGrid->InitGrid(m_iTerrainSize, m_iTerrainSize, 0.0f);
	m_pGeoMapGrid->CreateGeoMipGrid(m_iTerrainSize, m_iTerrainSize, m_iPatchSize, this);
}

void CBaseTerrain::InitializeShaders()
{
	m_pTerrainShader->AttachShader("shaders/terrain/terrain.vert");
	m_pTerrainShader->AttachShader("shaders/terrain/terrain.tcs");
	m_pTerrainShader->AttachShader("shaders/terrain/terrain.tes");
	m_pTerrainShader->AttachShader("shaders/terrain/terrain.frag");
	m_pTerrainShader->LinkPrograms();
}

GLfloat CBaseTerrain::GetWorldScale() const
{
	return (m_fWorldScale);
}

GLfloat CBaseTerrain::GetTextureScale() const
{
	return (m_fTextureScale);
}

GLfloat CBaseTerrain::GetHeight(GLint iX, GLint iZ) const
{
	return (m_pMapGrid->Get(iX, iZ));
}

GLint CBaseTerrain::GetSize() const
{
	return (m_iTerrainSize);
}

GLint CBaseTerrain::GetPatchSize() const
{
	return (m_iPatchSize);
}

GLint CBaseTerrain::GetWidth() const
{
	return (m_pMapGrid->GetWidth());
}

GLint CBaseTerrain::GetDepth() const
{
	return (m_pMapGrid->GetDepth());
}

CGrid<GLfloat>* CBaseTerrain::GetMapGrid()
{
	return (m_pMapGrid);
}

CShader* CBaseTerrain::GetTerrainShader()
{
	return (m_pTerrainShader);
}

CGeoMipGrid* CBaseTerrain::GetGeoMipGrid()
{
	return (m_pGeoMapGrid);
}

CWorldTranslation* CBaseTerrain::GetWorldTranslation()
{
	return (m_pWorldTranslation);
}

std::vector<CGeoMipGrid::TVertex>& CBaseTerrain::GetVertices() const
{
	return (m_pGeoMapGrid->GetVertices());
}

std::vector<GLuint>& CBaseTerrain::GetIndices() const
{
	return (m_pGeoMapGrid->GetIndices());
}

std::vector<TLodInfo>& CBaseTerrain::GetLodInfo() const
{
	return (m_pGeoMapGrid->GetLodInfo());
}

void CBaseTerrain::UpdateVertexBuffer()
{
	GetGeoMipGrid()->UpdateVertexBuffer();
}

void CBaseTerrain::SetLightDirection(const SVector3Df& v3LightDir)
{
	m_v3LightDir = v3LightDir;
	m_pTerrainShader->Use();
	SVector3Df ReversedLightDir = m_v3LightDir * -1.0f;
	ReversedLightDir = ReversedLightDir.normalize();
	m_pTerrainShader->setVec3("v3LightDirection", ReversedLightDir);
}

void CBaseTerrain::Render()
{
	auto rCamera = CCameraManager::Instance().GetCurrentCamera();

	// Bind SSBO to index 0
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_uiTerrainHandlesSSBO);

	// Render geometry
	m_pGeoMapGrid->Render(rCamera->GetPosition(), rCamera->GetViewProjMatrix());

	// Unbind SSBO from index 0
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0); // Critical for safety
}

void CBaseTerrain::Update()
{
	auto rCamera = CCameraManager::Instance().GetCurrentCamera();

	CMatrix4Df matVewProj = rCamera->GetViewProjMatrix();
	CMatrix4Df matView = rCamera->GetMatrix();

	SSceneElements* scene = CObject::pScene;
	m_v3LightDir = scene->v3LightDir;

	m_pTerrainShader->Use();

	// Vertex Shader
	m_pTerrainShader->setMat4("ViewMatrix", matVewProj);

	// Tessellation Control Shader
	m_pTerrainShader->setVec3("v3CameraPos", scene->pCamera->GetPosition());
	m_pTerrainShader->setFloat("fTessMultiplier", 0.5f);

	// Tessellation Evaluation Shader
	m_pTerrainShader->setMat4("mat4ViewProj", matVewProj);
	m_pTerrainShader->setVec3("v3CameraPos", scene->pCamera->GetPosition());
	SVector3Df v3PlaneNormal(0.0f, 1.0f, 0.0f);
	SVector3Df v3PointOnPlane(0.0f, 0.0f, 0.0f);
	float fDot = -v3PlaneNormal.dot(v3PointOnPlane);
	m_pTerrainShader->setVec4("v4ClipPlane", v3PlaneNormal.x, v3PlaneNormal.y, v3PlaneNormal.z, fDot);

	// Fragment Shader
	m_pTerrainShader->setVec3("v3LightDirection", scene->v3LightDir);
	m_pTerrainShader->setVec3("v3LightColor", scene->v3LightColor);
	m_pTerrainShader->setVec3("v3AmbientColor", SVector3Df(0.5f));
	m_pTerrainShader->setVec3("v3CameraPosition", scene->pCamera->GetPosition());
	m_pTerrainShader->setFloat("fShininess", 0.2f);
}

void CBaseTerrain::SetGUI()
{

}

void CBaseTerrain::SetTextureSet(CTerrainTextureSet* pTextureSet)
{
	static CTerrainTextureSet s_EmptyTextureSet;

	if (pTextureSet)
	{
		ms_pTerrainTextureSet = pTextureSet;
	}
	else
	{
		sys_err("CBaseTerrain::SetTextureSet: Passed an Empty Textureset!");
		ms_pTerrainTextureSet = &s_EmptyTextureSet;
	}

}

CTerrainTextureSet* CBaseTerrain::GetTextureSet()
{
	if (!ms_pTerrainTextureSet)
	{
		SetTextureSet(nullptr);
	}

	return (ms_pTerrainTextureSet);
}

size_t CBaseTerrain::GetTexturesCount() const
{
	return (ms_pTerrainTextureSet->GetTexturesCount());
}

const TTerrainTexture& CBaseTerrain::GetTerrainTexture(size_t iIndex) const
{
	return (ms_pTerrainTextureSet->GetTexture(iIndex));
}

void CBaseTerrain::AddTerrainTexture(const TTerrainTexture& terrainTex) const
{
	ms_pTerrainTextureSet->AddTexture(terrainTex);
}

void CBaseTerrain::SetCurrentTexture(size_t iIndex)
{
	ms_pTerrainTextureSet->GetTexture(iIndex);
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

float CBaseTerrain::GetWorldSize() const
{
	return (cast_floati(GetSize()) * m_fWorldScale);
}

/* Get Height by X - Z In World */
float CBaseTerrain::GetWorldHeight(GLfloat fX, GLfloat fZ) const
{
	const float fHeightMapX = fX / GetWorldScale();
	const float fHeightMapZ = fZ / GetWorldScale();

	return (GetHeightInterpolated(fHeightMapX, fHeightMapZ));
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

	v3NewCamPos.y = GetWorldHeight(v3CamPos.x, v3CamPos.z) + 35.0f;
	float fSmoothHeight = std::sinf(v3CamPos.x * 4.0f) + std::cosf(v3CamPos.z * 4.0f);
	fSmoothHeight /= 35.0f;

	v3NewCamPos.y += fSmoothHeight;

	CCameraManager::Instance().GetCurrentCamera()->SetPosition(v3NewCamPos);

	return (v3NewCamPos);
}

void CBaseTerrain::LoadTextureSet(const std::string& stFileName)
{
	ms_pTerrainTextureSet->Load(stFileName);
}

void CBaseTerrain::DoBindlesslyTexturesSetup()
{
	m_vTextureHandles.clear();

	// Collect handles from loaded textures
	for (auto& tex : ms_pTerrainTextureSet->GetTextures())
	{
		if (tex.m_pTexture)
		{
			m_vTextureHandles.push_back(tex.m_pTexture->GetHandle()); // Assuming CTexture has GetHandle()
		}
	}

	// Create and fill SSBO
	glGenBuffers(1, &m_uiTerrainHandlesSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_uiTerrainHandlesSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_vTextureHandles.size() * sizeof(GLuint64), m_vTextureHandles.data(), GL_STATIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_uiTerrainHandlesSSBO);

	// After creating SSBO
	GLuint64* handles = static_cast<GLuint64*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}
