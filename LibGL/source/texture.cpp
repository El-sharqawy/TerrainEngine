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
}

bool CTexture::Load()
{
	stbi_set_flip_vertically_on_load(1);

	auto* pImageData = stbi_load(m_strFullTexturePath.c_str(), &m_iWidth, &m_iHeight, &m_iChannelsBPP, 0);
	if (!pImageData)
	{
		sys_err("CTexture::Load Failed to load texture: '%s' - %s\n", m_strFullTexturePath.c_str(), stbi_failure_reason());
		return false;
	}

#if defined(ENABLE_PRINT_TEXTURE_DATA)
	printf("Texture: %s, Loaded with width: %d, height: %d, Channels: %d\n", m_stTextureName.c_str(), m_iWidth, m_iHeight, m_iChannelsBPP);
#endif
	LoadInternal(pImageData);
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

void CTexture::LoadRaw(GLint iWidth, GLint iHeight, GLint iChannelsBPP, const unsigned char* pImageData)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iChannelsBPP = iChannelsBPP;

	LoadInternal(pImageData);
}

void CTexture::LoadF32(GLint iWidth, GLint iHeight, const float* pImageData)
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

	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_BASE_LEVEL, 0);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_uiTextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

void CTexture::LoadInternal(const void* pImageData)
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

void CTexture::LoadInternalDSA(const void* pImageData)
{
	glCreateTextures(m_eTextureTarget, 1, &m_uiTextureID);

	GLint iLevels = std::min(5, (int)std::log2f((float)std::max(m_iWidth, m_iHeight)));
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

void CTexture::LoadInternalNonDSA(const void* pImageData)
{
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