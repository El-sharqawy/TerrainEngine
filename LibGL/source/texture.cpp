#include "stdafx.h"
#include "texture.h"

#define ENABLE_PRINT_TEXTURE_DATA

CTexture::CTexture(GLenum eTargetTexture)
{
	m_uiTextureID = 0;
	m_eTextureTarget = eTargetTexture;
	m_iWidth = 0;
	m_iHeight = 0;
	m_iChannelsBPP = 0;
	m_uiHandle = 0;
	m_IsResident = false;
}

CTexture::CTexture(const std::string& stFileName, GLenum eTargetTexture)
{
	m_uiTextureID = 0;
	m_eTextureTarget = eTargetTexture;
	m_iWidth = 0;
	m_iHeight = 0;
	m_iChannelsBPP = 0;
	m_strFullTexturePath = stFileName;
	m_fsFilePath = stFileName;
	if (std::filesystem::exists(m_fsFilePath))
	{
		if (m_fsFilePath.extension().empty() == false)
		{
			m_stTextureName = m_fsFilePath.stem().string();
		}
	}
	m_uiHandle = 0;
	m_IsResident = false;
}

CTexture::~CTexture()
{
	Clear();

	Destroy();
}

void CTexture::Destroy()
{
	glDeleteTextures(1, &m_uiTextureID);
	m_uiTextureID = 0;

	// Securely clear if sensitive data
	if (!m_vbTextureData.empty())
	{
		std::fill(m_vbTextureData.begin(), m_vbTextureData.end(), 0);
		m_vbTextureData.clear();
		m_vbTextureData.shrink_to_fit();
	}
}

bool CTexture::Load(bool bBindless)
{
	if (m_strFullTexturePath.empty())
	{
		sys_err("CTexture::Load Failed to Load Texture, Not Loaded File.");
		return false;
	}
	stbi_set_flip_vertically_on_load(1);

	auto* pImageData = stbi_load(m_strFullTexturePath.c_str(), &m_iWidth, &m_iHeight, &m_iChannelsBPP, 0);
	if (!pImageData)
	{
		sys_err("CTexture::Load Failed to load texture: '%s' - %s", m_strFullTexturePath.c_str(), stbi_failure_reason());
		return false;
	}

#if defined(ENABLE_PRINT_TEXTURE_DATA)
	sys_err("Texture: %s, Loaded with width: %d, height: %d, Channels: %d", m_stTextureName.c_str(), m_iWidth, m_iHeight, m_iChannelsBPP);
#endif

	m_vbTextureData.assign(pImageData, pImageData + m_iWidth * m_iHeight * m_iChannelsBPP);

	if (bBindless)
	{
		CreateBindlessTextureDSA(pImageData);
	}
	else
	{
		LoadInternal(pImageData);
	}
	return true;
}

void CTexture::Load(GLuint uiBufferSize, void* pImageData)
{
	void* pImageLoadedData = stbi_load_from_memory((const stbi_uc*)pImageData, uiBufferSize, &m_iWidth, &m_iHeight, &m_iChannelsBPP, 0);
	LoadInternal(pImageLoadedData);
	stbi_image_free(pImageLoadedData);
}

void CTexture::Load(const std::string& stFileName)
{
	m_strFullTexturePath = stFileName;
	m_fsFilePath = stFileName;
	if (std::filesystem::exists(m_fsFilePath))
	{
		if (m_fsFilePath.extension().empty() == false)
		{
			m_stTextureName = m_fsFilePath.stem().string();
		}
	}

	if (!Load())
	{
		sys_err("CTexture::Load Failed to load texture: '%s'\n", stFileName.c_str());
	}
}

void CTexture::LoadRaw(GLint iWidth, GLint iHeight, GLint iChannelsBPP, unsigned char* pImageData)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iChannelsBPP = iChannelsBPP;

	LoadInternal(pImageData);
}

void CTexture::LoadF32(GLint iWidth, GLint iHeight, float* pImageData)
{
	if (!IsGLVersionHigher(4, 5))
	{
		sys_err("Non DSA version is not implemented\n");
		return;
	}

	m_iWidth = iWidth;
	m_iHeight = iHeight;

	glCreateTextures(m_eTextureTarget, 1, &m_uiTextureID);
	glTextureStorage2D(m_uiTextureID, 1, GL_R32F, m_iWidth, m_iHeight);
	glTextureSubImage2D(m_uiTextureID, 0, 0, 0, m_iWidth, m_iHeight, GL_RED, GL_FLOAT, pImageData);

	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateTextureMipmap(m_uiTextureID);
}

void CTexture::Bind(GLenum eTextureUnit)
{
	if (IsGLVersionHigher(4, 5))
	{
		BindInternalDSA(eTextureUnit);
	}
	else
	{
		BindInternalNonDSA(eTextureUnit);
	}
}

void CTexture::GetImageSize(GLint& iImageWidth, GLint& iImageHeight)
{
	iImageWidth = m_iWidth;
	iImageHeight = m_iHeight;
}

GLuint CTexture::GetTextureID() const
{
	return m_uiTextureID;
}

GLenum CTexture::GetTextureTarget() const
{
	return (m_eTextureTarget);
}

GLuint64 CTexture::GetHandle() const
{
	return (m_uiHandle);
}

void CTexture::LoadInternal(void* pImageData)
{
	if (IsGLVersionHigher(4, 5))
	{
		LoadInternalDSA(pImageData);
	}
	else
	{
		LoadInternalNonDSA(pImageData);
	}
}

void CTexture::LoadInternalDSA(void* pImageData)
{
	const size_t dataSize = m_iWidth * m_iHeight * m_iChannelsBPP;
	m_vbTextureData.resize(dataSize);
	memcpy(m_vbTextureData.data(), pImageData, dataSize);

	glCreateTextures(m_eTextureTarget, 1, &m_uiTextureID);

	GLint iLevels = std::min(5, (GLint)std::log2f((GLfloat)std::max(m_iWidth, m_iHeight)));
	GLint SwizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_RED };

	if (m_eTextureTarget == GL_TEXTURE_2D)
	{
		switch (m_iChannelsBPP)
		{
		case 1:
			glTextureStorage2D(m_uiTextureID, iLevels, GL_R8, m_iWidth, m_iHeight);
			glTextureSubImage2D(m_uiTextureID, 0, 0, 0, m_iWidth, m_iHeight, GL_RED, GL_UNSIGNED_BYTE, pImageData);
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, SwizzleMask);
			break;

		case 2:
			glTextureStorage2D(m_uiTextureID, iLevels, GL_RG8, m_iWidth, m_iHeight);
			glTextureSubImage2D(m_uiTextureID, 0, 0, 0, m_iWidth, m_iHeight, GL_RG, GL_UNSIGNED_BYTE, pImageData);
			break;

		case 3:
			glTextureStorage2D(m_uiTextureID, iLevels, GL_RGB8, m_iWidth, m_iHeight);
			glTextureSubImage2D(m_uiTextureID, 0, 0, 0, m_iWidth, m_iHeight, GL_RGB, GL_UNSIGNED_BYTE, pImageData);
			break;

		case 4:
			glTextureStorage2D(m_uiTextureID, iLevels, GL_RGBA8, m_iWidth, m_iHeight);
			glTextureSubImage2D(m_uiTextureID, 0, 0, 0, m_iWidth, m_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
			break;

		default:
			sys_log("Not Implemented %d", m_iChannelsBPP);
			return;
		}
	}

	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateTextureMipmap(m_uiTextureID);
}

void CTexture::LoadInternalNonDSA(void* pImageData)
{
	const size_t dataSize = m_iWidth * m_iHeight * m_iChannelsBPP;
	m_vbTextureData.resize(dataSize);
	memcpy(m_vbTextureData.data(), pImageData, dataSize);

	glGenTextures(1, &m_uiTextureID);
	glBindTexture(m_eTextureTarget, m_uiTextureID);

	if (m_eTextureTarget == GL_TEXTURE_2D)
	{
		switch (m_iChannelsBPP)
		{
		case 1:
		{
			glTexImage2D(m_eTextureTarget, 0, GL_RED, m_iWidth, m_iHeight, 0, GL_RED, GL_UNSIGNED_BYTE, pImageData);
			GLint SwizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_RED };
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, SwizzleMask);
		}
		break;

		case 2:
			glTexImage2D(m_eTextureTarget, 0, GL_RG, m_iWidth, m_iHeight, 0, GL_RG, GL_UNSIGNED_BYTE, pImageData);
			break;

		case 3:
			glTexImage2D(m_eTextureTarget, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageData);
			break;

		case 4:
			glTexImage2D(m_eTextureTarget, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
			break;

		default:
			sys_log("Not Implemented %d", m_iChannelsBPP);
			return;
		}
	}

	glTexParameteri(m_eTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(m_eTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_eTextureTarget, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(m_eTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_eTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(m_eTextureTarget);
	glBindTexture(m_eTextureTarget, 0);
}

void CTexture::BindInternalDSA(GLenum eTextureUnit)
{
	glBindTextureUnit(eTextureUnit - GL_TEXTURE0, m_uiTextureID);
}

void CTexture::BindInternalNonDSA(GLenum eTextureUnit)
{
	glActiveTexture(eTextureUnit);
	glBindTexture(m_eTextureTarget, m_uiTextureID);
}

GLuint CTexture::GenerateTexture2D(GLint iWidth, GLint iHeight)
{
	if (IsGLVersionHigher(4, 5))
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_uiTextureID);
	}
	else
	{
		glGenTextures(1, &m_uiTextureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
	}

	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateTextureMipmap(m_uiTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, iWidth, iHeight, 0, GL_RGBA, GL_FLOAT, nullptr);

	glBindImageTexture(0, m_uiTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	return (m_uiTextureID);
}

GLuint CTexture::GenerateEmptyTexture2D(GLint iWidth, GLint iHeight)
{
	glGenTextures(1, &m_uiTextureID);
	glBindTexture(m_eTextureTarget, m_uiTextureID);

	glTexImage2D(m_eTextureTarget, 0, GL_RGBA8, iWidth, iHeight, 0, GL_RGBA, GL_FLOAT, nullptr);

	glTexParameteri(m_eTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_eTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_eTextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_eTextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(m_eTextureTarget, 0);

	return (m_uiTextureID);
}

GLuint CTexture::GenerateColoredTexture2D(GLint iWidth, GLint iHeight, const SVector4Df& v4Color)
{
	// Step 1: Generate and bind the texture
	glGenTextures(1, &m_uiTextureID);
	glBindTexture(m_eTextureTarget, m_uiTextureID);

	glTexImage2D(m_eTextureTarget, 0, GL_RGBA8, iWidth, iHeight, 0, GL_RGBA, GL_FLOAT, nullptr);

	// Step 2: Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Step 3: Create a buffer with the solid color (e.g., green with alpha)
	std::vector<GLfloat> colorData(iWidth * iHeight * 4);

	for (size_t i = 0; i < iWidth * iHeight; ++i)
	{
		colorData[i * 4 + 0] = v4Color.r; // R
		colorData[i * 4 + 1] = v4Color.g; // G
		colorData[i * 4 + 2] = v4Color.b; // B
		colorData[i * 4 + 3] = v4Color.a; // A
	}

	// Allocate and fill texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_FLOAT, colorData.data());

	glBindTexture(m_eTextureTarget, 0);
	return (m_uiTextureID);
}

GLuint CTexture::GenerateTexture3D(GLint iWidth, GLint iHeight, GLint iDepth)
{
	if (IsGLVersionHigher(4, 5))
	{
		glCreateTextures(GL_TEXTURE_3D, 1, &m_uiTextureID);
	}
	else
	{
		glGenTextures(1, &m_uiTextureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, m_uiTextureID);
	}

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, iWidth, iHeight, iDepth, 0, GL_RGBA, GL_FLOAT, nullptr);
	glGenerateMipmap(GL_TEXTURE_3D);

	glBindImageTexture(0, m_uiTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	return (m_uiTextureID);
}

void CTexture::BindTexture2D(GLuint iUnit)
{
	glBindImageTexture(iUnit, m_uiTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
}

void CTexture::BindTexture3D(GLuint iUnit)
{
	glBindImageTexture(iUnit, m_uiTextureID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
}

void CTexture::MakeResident()
{
	if (m_uiHandle == 0)
	{
		m_uiHandle = glGetTextureHandleARB(m_uiTextureID);
	}
	if (!glIsTextureHandleResidentARB(m_uiHandle))
	{
		glMakeTextureHandleResidentARB(m_uiHandle);
	}

	m_IsResident = true;
}

void CTexture::MakeNonResident()
{
	if (m_IsResident)
	{
		glMakeTextureHandleNonResidentARB(m_uiHandle);
		m_IsResident = false;
	}
}

void CTexture::CreateBindlessTextureDSA(void* pImageData)
{
	// Calculate full mipmap chain
	const GLint maxLevel = static_cast<GLint>(std::floor(std::log2(std::max(m_iWidth, m_iHeight))) + 1);

	// Set default swizzle mask
	GLint swizzleMask[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA };

	glCreateTextures(m_eTextureTarget, 1, &m_uiTextureID);

	GLenum internalFormat = GL_RGBA8;
	GLenum format = GL_RGBA;

	switch (m_iChannelsBPP) {
	case 1:
		internalFormat = GL_R8;
		format = GL_RED;
		swizzleMask[1] = GL_RED;  // Duplicate red to RGB
		swizzleMask[2] = GL_RED;
		swizzleMask[3] = GL_ONE;   // Set alpha to 1.0
		break;

	case 2:
		internalFormat = GL_RG8;
		format = GL_RG;
		break;

	case 3:
		// Change from sRGB to normal RGB
		internalFormat = GL_RGB8;        // Changed from GL_SRGB8
		format = GL_RGB;
		break;

	case 4:
		internalFormat = GL_RGBA8;       // Changed from GL_SRGB8_ALPHA8 
		format = GL_RGBA;
		break;
	}

	// Allocate immutable storage
	glTextureStorage2D(m_uiTextureID, maxLevel, internalFormat, m_iWidth, m_iHeight);

	// Upload pixel data
	glTextureSubImage2D(m_uiTextureID, 0, 0, 0, m_iWidth, m_iHeight, format, GL_UNSIGNED_BYTE, pImageData);

	// Set texture parameters
	glTextureParameteriv(m_uiTextureID, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameterf(m_uiTextureID, GL_TEXTURE_LOD_BIAS, -0.5f);

	glTextureParameteri(m_uiTextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Generate complete mipmap chain
	glGenerateTextureMipmap(m_uiTextureID);

	// Set mipmap range
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MAX_LEVEL, maxLevel - 1);

	// Optional: set anisotropic filtering
	GLfloat maxAniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
	glTextureParameterf(m_uiTextureID, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);
}

/////////////////////// TEXTURESET /////////////////////
CTextureSet::CTextureSet(GLint iWidth, GLint iHeight, size_t iTexNum)
{
	m_iNumTextures = iTexNum;

	if (iWidth <= 0 || iHeight <= 0 || iTexNum <= 0)
	{
		sys_err("CTextureSet::CTextureSet Failed to Initialize TextureSets! (%d, %d, %zu)", iWidth, iHeight, iTexNum);
		return;
	}

	m_vTextures.resize(iTexNum);

	for (size_t i = 0; i < m_vTextures.size(); i++)
	{
		m_vTextures[i] = new CTexture();
		m_vTextures[i]->GenerateTexture2D(iWidth, iHeight);
	}
}

CTextureSet::~CTextureSet()
{
	for (auto& it : m_vTextures)
	{
		safe_delete(it);
	}

	m_vTextures.clear();
}

void CTextureSet::BindTexture(size_t iNum, GLuint iUnit)
{
	m_vTextures[iNum]->BindTexture2D(iUnit);
}

GLuint CTextureSet::GetColorAttachmentTextureID(size_t iNum)
{
	if (iNum >= m_vTextures.size())
	{
		sys_err("CTextureSet::GetColorAttachmentTextureID trying to access member out of vector range! (%zu - %zu)", iNum, m_vTextures.size());
		return 0;
	}

	return m_vTextures[iNum]->GetTextureID();
}

CTexture* CTextureSet::GetColorAttachmentTexture(size_t iNum)
{
	if (iNum >= m_vTextures.size())
	{
		sys_err("CTextureSet::GetColorAttachmentTexture trying to access member out of vector range! (%zu - %zu)", iNum, m_vTextures.size());
		return 0;
	}

	return m_vTextures[iNum];
}

void CTextureSet::BindTextures()
{
	for (GLuint i = 0; i < m_vTextures.size(); i++)
	{
		m_vTextures[i]->BindTexture2D(i);
	}
}

