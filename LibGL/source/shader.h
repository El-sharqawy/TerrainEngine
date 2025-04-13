#pragma once

#include "stdafx.h"
#include <string>
#include "base_shader.h"

class CShader
{
public:
	/* constructor reads and builds the shader */
	CShader(const std::string& stName);
	CShader(const std::string& stName, const std::string& stComputeShader);

	virtual ~CShader();

	CShader* AttachShader(const CBaseShader& shader);

	void LinkPrograms();

	/* the program ID */
	GLuint GetID() const;

	/* use / activate Shader */
	void Use() const;

	bool IsCompute() const;
	bool IsLinked() const;

	std::string GetName() const;

	/* utility uniform functions */
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, GLint value) const;
	void setFloat(const std::string& name, float value) const;
	void set2Float(const std::string& name, float value1, float value2) const;
	void setVec2(const std::string& name, const glm::vec2& vec2) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& vec3) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec4(const std::string& name, const glm::vec4& vec4) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2& matrix) const;
	void setMat3(const std::string& name, const glm::mat3& matrix) const;
	void setMat4(const std::string& name, const glm::mat4& matrix) const;

	/* utility uniform functions for my classes */
	void setVec2(const std::string& name, const SVector2Df& vec2) const;
	void setVec3(const std::string& name, const SVector3Df& vec3) const;
	void setVec4(const std::string& name, const SVector4Df& vec4) const;
	void setMat4(const std::string& name, const CMatrix4Df& matrix, bool bTranspose = GL_TRUE) const;

	void setSampler2D(const std::string& name, GLuint iTextureID, GLint iTexValue) const;
	void setSampler3D(const std::string& name, GLuint iTextureID, GLint iTexValue) const;

protected:
	/* the program ID */
	GLuint m_uiID{0}; // Recommended: ensures m_uiID is always valid.;

	std::list<GLuint> m_lShaders;
	std::string m_stName;

	bool m_bIsLinked;
	bool m_bIsCompute;
};