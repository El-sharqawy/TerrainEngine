#include "stdafx.h"
#include "texture_set.h"
#include <fstream>

CTerrainTextureSet::CTerrainTextureSet()
{
	Create();
}

CTerrainTextureSet::~CTerrainTextureSet()
{
	Clear();
}

void CTerrainTextureSet::Clear()
{
	safe_delete(m_tErrorTexture.m_pTexture);
	for (auto& it : m_vTextures)
	{
		safe_delete(it.m_pTexture);
	}
	m_vTextures.clear();
}

void CTerrainTextureSet::Create()
{
	m_tErrorTexture.m_pTexture = new CTexture("resources/textureset/error.png", GL_TEXTURE_2D);
	m_tErrorTexture.m_pTexture->Load();
	m_tErrorTexture.m_pTexture->MakeResident();
	m_tErrorTexture.m_uiTextureID = m_tErrorTexture.m_pTexture->GetTextureID();

	m_fTerrainTexCoordBase = 16.0f;

	AddEmptyTexture();
}

void CTerrainTextureSet::AddEmptyTexture()
{
	TTerrainTexture EraserTexture;
	EraserTexture.m_pTexture = new CTexture(GL_TEXTURE_2D);
	EraserTexture.m_pTexture->GenerateColoredTexture2D(TERRAIN_PATCH_SIZE, TERRAIN_PATCH_SIZE, SVector4Df(0.0f, 1.0f, 0.3f, 0.1f));
	EraserTexture.m_pTexture->MakeResident();
	m_vTextures.emplace_back(EraserTexture);
}

CTexture* CTerrainTextureSet::GetEraserTexture()
{
	assert(m_vTextures[0].m_pTexture);
	return (m_vTextures[0].m_pTexture);
}

size_t CTerrainTextureSet::GetTexturesCount()
{
	return (m_vTextures.size());
}

TTerrainTexture& CTerrainTextureSet::GetTexture(size_t iIndex)
{
	if (iIndex >= GetTexturesCount())
	{
		sys_err("CTerrainTextureSet::GetTexture: Index %zu out of textures vector range (%zu), Return Error texture", iIndex, GetTexturesCount());
		return (m_tErrorTexture);
	}

	return (m_vTextures[iIndex]);
}

bool CTerrainTextureSet::RemoveTexture(size_t iIndex)
{
	if (iIndex >= GetTexturesCount())
	{
		sys_err("CTerrainTextureSet::RemoveTexture: Index %zu out of textures vector range (%zu)", iIndex, GetTexturesCount());
		return (false);
	}

	// Extract entry FIRST
	TTerrainTexture texture = m_vTextures[iIndex];
	safe_delete(texture.m_pTexture);

	m_vTextures.erase(m_vTextures.begin() + iIndex);

	// Now delete safely

	return (true);
}

bool CTerrainTextureSet::Save(const std::string& stFileName)
{
	json jsonTexture;
	jsonTexture["texture_count"] = GetTexturesCount() - 1; // remove Eraser
	jsonTexture["texture_set"] = json::array();

	// Convert all textures to JSON objects
	for (size_t i = 0; i < GetTexturesCount(); i++)
	{
		// Skip first texture, it's eraser.
		if (i == 0)
		{
			continue;
		}

		json textureEntry{};
		textureEntry["texture_id"] = i;
		textureEntry["textures"] = m_vTextures[i].ToJson();
		jsonTexture["texture_set"].push_back(textureEntry);
	}

	try
	{
		std::ofstream file(stFileName);
		file << std::setw(4) << jsonTexture << std::endl;
		return (true);
	}
	catch (const std::exception& e)
	{
		sys_err("CTerrainTextureSet::Save: Failed to Save the file %s, error: %s", stFileName.c_str(), e.what());
		return (false);
	}

	return (true);
}

bool CTerrainTextureSet::Load(const std::string& stFileName)
{
	Clear();

	// Open and read file
	std::ifstream file(stFileName);
	if (!file.is_open())
	{
		sys_err("CTerrainTextureSet::Load: Failed to open file: %s", stFileName.c_str());
		return false;
	}

	try
	{
		// Parse JSON
		json jsonData;

		try
		{
			file >> jsonData;
		}
		catch (const json::parse_error& e)
		{
			// logger.error("JSON parse error: {}", e.what());
			sys_err("CTerrainTextureSet::Load: JSON parse error for File %s, error: %s", stFileName.c_str(), e.what());
			return false;
		}

		if (!jsonData.contains("texture_set") || !jsonData["texture_set"].is_array())
		{
			// logger.error("Invalid texture file format");
			sys_err("CTerrainTextureSet::Load: JSON parse error for File %s, error: Invalid texture file format", stFileName.c_str());
			return false;
		}

		// Clear existing textures (keep eraser at index 0)
		Create();

		size_t textures_count = jsonData["texture_count"]; // remove Eraser

		m_vTextures.resize(textures_count + 1); // +1 for the eraser at index 0

		// Load textures from JSON
		for (size_t i = 0; i < textures_count; ++i)
		{
			TTerrainTexture texture;
			const auto& jsonTex = jsonData["texture_set"][i];

			GLuint texture_id = jsonTex["texture_id"].get<GLuint>(); // Element ID

			const auto& data = jsonTex["textures"]; // correct key

			texture.m_stFileName = data["filename"].get<std::string>();
			texture.m_uiTextureID = texture_id;
			texture.m_fUScale = data["u_scale"].get<GLfloat>();
			texture.m_fVScale = data["v_scale"].get<GLfloat>();
			texture.m_fUOffset = data["u_offset"].get<GLfloat>();
			texture.m_fVOffset = data["v_offset"].get<GLfloat>();
			texture.m_bIsSplat = data["is_splat"].get<bool>();
			texture.m_uiHeightMin = data["begin"].get<GLuint>();
			texture.m_uiHeightMax = data["end"].get<GLuint>();

			SetTexture(i + 1, texture);
		}

		m_stFileName.assign(stFileName);

		sys_log("CTerrainTextureSet::Load: Succeed To Load Textureset %s", m_stFileName.c_str());
		return true;
	}
	catch (const std::exception& e)
	{
		sys_err("CTerrainTextureSet::Load: Failed to Load the file %s, error: %s", stFileName.c_str(), e.what());
		return (false);
	}
}

void CTerrainTextureSet::Reload()
{
	for (size_t i = 1; i < m_vTextures.size(); ++i)
	{
		TTerrainTexture& tex = m_vTextures[i];

		// Reset and reload the texture
		if (tex.m_pTexture)
		{
			safe_delete(tex.m_pTexture);
		}

		tex.m_pTexture = new CTexture(tex.m_stFileName, GL_TEXTURE_2D);

		if (!tex.m_pTexture->Load())
		{
			sys_err("CTerrainTextureSet::Reload: Failed to reload texture '%s'", tex.m_stFileName.c_str());
			safe_delete(tex.m_pTexture);  // Clear invalid texture
			continue;
		}

		tex.m_pTexture->MakeResident();

		tex.m_uiTextureID = tex.m_pTexture->GetTextureID();

		// Recompute the transformation matrix
		CMatrix4Df matScale{};
		matScale.InitScaleTransform(m_fTerrainTexCoordBase * tex.m_fUScale, -m_fTerrainTexCoordBase * tex.m_fVScale, 1.0f);

		CMatrix4Df matTranslate{};
		matTranslate.InitTranslationTransform(tex.m_fUOffset, -tex.m_fVOffset, 0.0f);

		tex.m_matTransform = matTranslate * matScale;
	}
}

bool CTerrainTextureSet::SetTexture(size_t iIndex, const TTerrainTexture& Texture)
{
	if (iIndex >= m_vTextures.size())
	{
		sys_err("CTerrainTextureSet::GetTexture: Index %zu out of textures vector range (%zu), return error texture", iIndex, GetTexturesCount());
		return (false);
	}

	TTerrainTexture& tex = m_vTextures[iIndex];

	tex.m_stFileName = Texture.m_stFileName;
	tex.m_uiTextureID = Texture.m_uiTextureID;
	tex.m_fUScale = Texture.m_fUScale;
	tex.m_fVScale = Texture.m_fVScale;
	tex.m_fUOffset = Texture.m_fUOffset;
	tex.m_fVOffset = Texture.m_fVOffset;
	tex.m_bIsSplat = Texture.m_bIsSplat;
	tex.m_uiHeightMin = Texture.m_uiHeightMin;
	tex.m_uiHeightMax = Texture.m_uiHeightMax;

	// Create and load the new texture
	tex.m_pTexture = new CTexture(tex.m_stFileName, GL_TEXTURE_2D);
	if (!tex.m_pTexture->Load())
	{
		sys_err("CTerrainTextureSet::SetTexture: Failed to load texture '%s'", tex.m_stFileName.c_str());
		safe_delete(tex.m_pTexture);  // Clear invalid texture
		return false;
	}

	tex.m_pTexture->MakeResident();

	tex.m_uiTextureID = tex.m_pTexture->GetTextureID();

	// Calculate transformation matrix
	CMatrix4Df matScale{};
	matScale.InitScaleTransform(m_fTerrainTexCoordBase * tex.m_fUScale, -m_fTerrainTexCoordBase * tex.m_fVScale, 1.0f);

	CMatrix4Df matTranslate{};
	matTranslate.InitTranslationTransform(tex.m_fUOffset, -tex.m_fVOffset, 0.0f);

	tex.m_matTransform = matTranslate * matScale;

	return (true);
}

bool CTerrainTextureSet::SetTexture(size_t iIndex,
	const std::string& stFileName,
	GLfloat fUScale,
	GLfloat fVScale,
	GLfloat fUOffset,
	GLfloat fVOffset,
	bool bIsSplat,
	GLuint uiHeightMin,
	GLuint uiHeightMax)
{
	if (iIndex >= m_vTextures.size())
	{
		sys_err("CTerrainTextureSet::GetTexture: Index %zu out of textures vector range (%zu), return error texture", iIndex, GetTexturesCount());
		return (false);
	}

	TTerrainTexture& tex = m_vTextures[iIndex];

	tex.m_stFileName = stFileName;
	tex.m_uiTextureID = 0;
	tex.m_fUScale = fUScale;
	tex.m_fVScale = fVScale;
	tex.m_fUOffset = fUOffset;
	tex.m_fVOffset = fVOffset;
	tex.m_bIsSplat = bIsSplat;
	tex.m_uiHeightMin = uiHeightMin;
	tex.m_uiHeightMax = uiHeightMax;

	// Create and load the new texture
	tex.m_pTexture = new CTexture(tex.m_stFileName, GL_TEXTURE_2D);
	if (!tex.m_pTexture->Load())
	{
		sys_err("CTerrainTextureSet::SetTexture: Failed to load texture '%s'", tex.m_stFileName.c_str());
		safe_delete(tex.m_pTexture);  // Clear invalid texture
		return false;
	}

	tex.m_pTexture->MakeResident();

	tex.m_uiTextureID = tex.m_pTexture->GetTextureID();

	// Calculate transformation matrix
	CMatrix4Df matScale{};
	matScale.InitScaleTransform(m_fTerrainTexCoordBase * tex.m_fUScale, -m_fTerrainTexCoordBase * tex.m_fVScale, 1.0f);

	CMatrix4Df matTranslate{};
	matTranslate.InitTranslationTransform(tex.m_fUOffset, -tex.m_fVOffset, 0.0f);

	tex.m_matTransform = matTranslate * matScale;

	return (true);
}

bool CTerrainTextureSet::AddTexture(const TTerrainTexture& Texture)
{
	if (GetTexturesCount() >= 256)
	{
		sys_err("CTerrainTextureSet::AddTexture: No more textures can be added, Maximum Number is 255 (cur: %zu)", GetTexturesCount());
		return (false);
	}

	for (const auto& tex : m_vTextures)
	{
		if (tex.m_stFileName == Texture.m_stFileName)
		{
			sys_err("CTerrainTextureSet::AddTexture: Failed to Add %s A texture with the same name already exists.", Texture.m_stFileName.c_str());
			return (false);
		}
	}

	if (!IsValidImage(Texture.m_stFileName))
	{
		sys_err("CTerrainTextureSet::AddTexture: %s Is not a Valid Texture, Error: %s", Texture.m_stFileName.c_str(), stbi_failure_reason() ? stbi_failure_reason() : "Image not Supported");
		return (false);
	}

	m_vTextures.reserve(m_vTextures.size() + 1);

	return (SetTexture(m_vTextures.size() - 1, Texture));
}

bool CTerrainTextureSet::AddTexture(const std::string& stFileName,
	GLfloat fUScale,
	GLfloat fVScale,
	GLfloat fUOffset,
	GLfloat fVOffset,
	bool bIsSplat,
	GLuint uiHeightMin,
	GLuint uiHeightMax)
{
	if (GetTexturesCount() >= 256)
	{
		sys_err("CTerrainTextureSet::AddTexture: No more textures can be added, Maximum Number is 255 (cur: %zu)", GetTexturesCount());
		return (false);
	}

	for (const auto& tex : m_vTextures)
	{
		if (tex.m_stFileName == stFileName)
		{
			sys_err("CTerrainTextureSet::AddTexture: Failed to Add %s A texture with the same name already exists.", stFileName.c_str());
			return (false);
		}
	}

	if (!IsValidImage(stFileName))
	{
		sys_err("CTerrainTextureSet::AddTexture: %s Is not a Valid Texture, Error: %s", stFileName.c_str(), stbi_failure_reason() ? stbi_failure_reason() : "Image not Supported");
		return (false);
	}

	// m_vTextures.reserve(m_vTextures.size() + 1); -> keeps only last element (changing it)?
	m_vTextures.emplace_back();

	return (SetTexture(m_vTextures.size() - 1, stFileName, fUScale, fVScale, fUOffset, fVOffset, bIsSplat, uiHeightMin, uiHeightMax));
}

bool CTerrainTextureSet::IsValidImage(const std::string& filePath)
{
	// Check file existence and regular file status
	if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath))
	{
		sys_err("CTerrainTextureSet::IsValidImage: Failed to Find the Texture %s", filePath.c_str());
		return false;
	}

	int width, height, channels;
	const bool isValid = stbi_info(filePath.c_str(), &width, &height, &channels);

	return (isValid);
}

void CTerrainTextureSet::ResizeTextures(size_t iNum)
{
	m_vTextures.resize(iNum);
}

void CTerrainTextureSet::IncreaseTexturesNum(size_t iNum)
{
	m_vTextures.resize(m_vTextures.size() + iNum);
}

CTerrainTextureSet::TTexturesVector& CTerrainTextureSet::GetTextures()
{
	return (m_vTextures);
}
