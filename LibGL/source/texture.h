#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <string>
#include <filesystem> // C++17

class CTexture
{
public:
	CTexture(const std::string& stFileName, GLenum eTargetTexture);
	CTexture(GLenum eTargetTexture = GL_TEXTURE_2D);
	~CTexture();

	void Destroy();

	// Prevent copying
	CTexture(const CTexture&) = delete;
	CTexture& operator=(const CTexture&) = delete;

	// Move constructor
	CTexture(CTexture&& other) noexcept
		: m_uiTextureID(other.m_uiTextureID),
		m_vbTextureData(std::move(other.m_vbTextureData)),
		m_iWidth(other.m_iWidth),
		m_iHeight(other.m_iHeight),
		m_iChannelsBPP(other.m_iChannelsBPP),
		m_eTextureTarget(other.m_eTextureTarget)
	{
		// Reset source object's OpenGL ID to prevent double deletion
		other.m_uiTextureID = 0;
		other.Clear();
	}

	// Move assignment operator
	CTexture& operator=(CTexture&& other) noexcept
	{
		if (this != &other)
		{
			Destroy();

			// Transfer resources
			m_uiTextureID = other.m_uiTextureID;
			m_vbTextureData = std::move(other.m_vbTextureData);
			m_iWidth = other.m_iWidth;
			m_iHeight = other.m_iHeight;
			m_iChannelsBPP = other.m_iChannelsBPP;
			m_eTextureTarget = other.m_eTextureTarget;

			// Invalidate source
			other.m_uiTextureID = 0;
			other.Clear();
		}
		return *this;
	}

	// Should be called once to load the texture
	bool Load(bool bBindless = false);

	void Load(GLuint uiBufferSize, void* pImageData);

	void Load(const std::string& stFileName);

	void LoadRaw(GLint iWidth, GLint iHeight, GLint iChannelsBPP, unsigned char* pImageData);

	void LoadF32(GLint iWidth, GLint iHeight, float* pImageData);

	// Should be called once to bind the texture
	void Bind(GLenum eTextureUnit);
	void GetImageSize(GLint& iImageWidth, GLint& iImageHeight);
	GLuint GetTextureID() const;
	GLenum GetTextureTarget() const;
	GLuint64 GetHandle() const;

	GLuint GenerateTexture2D(GLint iWidth, GLint iHeight);
	GLuint GenerateTexture3D(GLint iWidth, GLint iHeight, GLint iDepth);
	GLuint GenerateEmptyTexture2D(GLint iWidth, GLint iHeight, GLint iTextureType);
	GLuint GenerateColoredTexture2D(GLint iWidth, GLint iHeight, const SVector4Df& v4Color);

	void BindTexture2D(GLuint iUnit);
	void BindTexture3D(GLuint iUnit);

	void MakeResident();
	void MakeNonResident();

	void UploadFloatRGBA(const SVector4Df* data);


private:
	void LoadInternal(void* pImageData);
	void LoadInternalDSA(void* pImageData);
	void LoadInternalNonDSA(void* pImageData);

	void BindInternalDSA(GLenum eTextureUnit);
	void BindInternalNonDSA(GLenum eTextureUnit);

	void CreateBindlessTextureDSA(void* pImageData);

	void Clear()
	{
		m_iWidth = 0;
		m_iHeight = 0;
		m_iChannelsBPP = 0;
		m_eTextureTarget = GL_TEXTURE_2D;
		m_uiHandle = 0;
		m_IsResident = false;
	}

private:
	GLuint m_uiTextureID;
	GLenum m_eTextureTarget;
	std::string m_stTextureName;
	std::string m_strFullTexturePath;
	std::filesystem::path m_fsFilePath;
	GLint m_iWidth;
	GLint m_iHeight;
	GLint m_iChannelsBPP;
	std::vector<GLubyte> m_vbTextureData; // Stores raw pixel data

	// For Bindless Textures
	GLuint64 m_uiHandle = 0;
	bool m_IsResident = false;
};

class CTextureSet
{
	CTextureSet(GLint iWidth, GLint iHeight, size_t iTexNum);
	~CTextureSet();

	void BindTexture(size_t iNum, GLuint iUnit);
	GLuint GetColorAttachmentTextureID(size_t iNum);
	CTexture* GetColorAttachmentTexture(size_t iNum);
	void BindTextures();

private:
	size_t m_iNumTextures;
	std::vector<CTexture*> m_vTextures;
};