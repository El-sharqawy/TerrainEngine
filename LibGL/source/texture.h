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

	// Should be called once to load the texture
	bool Load();

	void Load(GLuint uiBufferSize, void* pImageData);

	void Load(const std::string& stFileName);

	void LoadRaw(GLint iWidth, GLint iHeight, GLint iChannelsBPP, const unsigned char* pImageData);

	void LoadF32(GLint iWidth, GLint iHeight, const float* pImageData);

	// Should be called once to bind the texture
	void Bind(GLenum eTextureUnit);
	void GetImageSize(GLint& iImageWidth, GLint& iImageHeight);
	GLuint GetTextureID() const;

private:
	void LoadInternal(const void* pImageData);
	void LoadInternalDSA(const void* pImageData);
	void LoadInternalNonDSA(const void* pImageData);

	void BindInternalDSA(GLenum eTextureUnit);
	void BindInternalNonDSA(GLenum eTextureUnit);

private:
	GLuint m_uiTextureID;
	GLenum m_eTextureTarget;
	std::string m_stTextureName;
	std::string m_strFullTexturePath;
	std::filesystem::path m_fsFilePath;
	GLint m_iWidth;
	GLint m_iHeight;
	GLint m_iChannelsBPP;
};