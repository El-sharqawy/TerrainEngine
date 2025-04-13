#pragma once

#include <glad/glad.h>
#include <string>

typedef struct SShaderType
{
	std::string m_stName;
	GLuint m_uiType;

	SShaderType()
	{
		m_stName = "Shader";
		m_uiType = 0;
	}

	SShaderType(const std::string& stName, GLuint uiType)
	{
		m_stName = stName;
		m_uiType = uiType;
	}
} TShaderType;

class CBaseShader
{
public:
	CBaseShader(const std::string& stShaderPath);
	CBaseShader(const char* stShaderPath);

	virtual ~CBaseShader() = default;

	std::string GetShaderFullPath() const;
	GLuint GetShaderID() const;
	std::string GetTypeName() const;
	std::string GetName() const;
	std::string GetRealType() const;
	TShaderType GetType() const;

protected:
	std::string LoadShaderFromFile(const std::string& stShaderPath);

private:
	std::string m_stPath;
	GLuint m_uiShaderID;
	TShaderType m_sShaderType;
};

extern bool CheckCompileErrors(GLuint uiShaderID, const std::string& stShaderType, const std::string& stShaderName);
extern std::string GetShaderName(const std::string& stShaderPath);
extern std::string GetShaderTypeName(const std::string& stShaderPath);
extern TShaderType GetShaderType(const std::string& stShaderPath);
