#include "stdafx.h"
#include "base_shader.h"
#include <fstream>
#include <sstream>

CBaseShader::CBaseShader(const std::string& stShaderPath)
{
	m_stPath = stShaderPath;
	std::string shaderCode = LoadShaderFromFile(stShaderPath);
	const char* shaderCodeStr = shaderCode.c_str();

	m_sShaderType = GetShaderType(stShaderPath);
	m_uiShaderID = glCreateShader(m_sShaderType.m_uiType);

	glShaderSource(m_uiShaderID, 1, &shaderCodeStr, nullptr);
	glCompileShader(m_uiShaderID);
	CheckCompileErrors(m_uiShaderID, m_sShaderType.m_stName, GetShaderName(stShaderPath));
}

CBaseShader::CBaseShader(const char* stShaderPath)
{
	m_stPath = std::string(stShaderPath);
	std::string shaderCode = LoadShaderFromFile(std::string(stShaderPath));
	const char* shaderCodeStr = shaderCode.c_str();

	m_sShaderType = GetShaderType(std::string(stShaderPath));
	m_uiShaderID = glCreateShader(m_sShaderType.m_uiType);

	glShaderSource(m_uiShaderID, 1, &shaderCodeStr, nullptr);
	glCompileShader(m_uiShaderID);
	CheckCompileErrors(m_uiShaderID, m_sShaderType.m_stName, GetShaderName(std::string(stShaderPath)));

	sys_log("CBaseShader::CBaseShader ShaderName: %s, ShaderID: %d, ShaderType: %s", GetName().c_str(), GetShaderID(), GetType().m_stName.c_str());
}

std::string CBaseShader::LoadShaderFromFile(const std::string& stShaderPath)
{
	/* 1. retrieve the shader source code from filePath */
	std::string shaderCode;
	std::ifstream fShaderFile; /* shader file */

	/* ensure ifstream objects can throw exceptions: */
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		/* Open Files */
		fShaderFile.open(stShaderPath);

		std::stringstream sShaderStream;

		/* read file’s buffer contents into streams */
		sShaderStream << fShaderFile.rdbuf();

		/* close file handlers */
		fShaderFile.close();

		/* convert streams into strings */
		shaderCode = sShaderStream.str();

	}
	catch (std::ifstream::failure err)
	{
		sys_err("CBaseShader::LoadShaderFromFile Failed to Load the Shader File %s", GetShaderName(stShaderPath).c_str());
	}

	return (shaderCode);
}

std::string CBaseShader::GetShaderFullPath() const
{
	return (m_stPath);
}

GLuint CBaseShader::GetShaderID() const
{
	return (m_uiShaderID);
}

std::string CBaseShader::GetTypeName() const
{
	return (GetShaderTypeName(GetShaderFullPath()));
}

std::string CBaseShader::GetName() const
{
	return (GetShaderName(GetShaderFullPath()));
}

std::string CBaseShader::GetRealType() const
{
	return (GetType().m_stName);
}

TShaderType CBaseShader::GetType() const
{
	return (m_sShaderType);
}

bool CheckCompileErrors(GLuint uiShaderID, const std::string& stShaderType, const std::string& stShaderName)
{
	GLint iSuccess = 0;
	char c_szInfoLog[1024];

	if (stShaderType != "program")
	{
		glGetShaderiv(uiShaderID, GL_COMPILE_STATUS, &iSuccess);
		if (!iSuccess)
		{
			glGetShaderInfoLog(uiShaderID, 1024, nullptr, c_szInfoLog);
			sys_err("CBaseShader::CheckCompileErrors Error shader: %s, Compilation Error of Type %s", stShaderName.c_str(), stShaderType.c_str());
			sys_err("CBaseShader::CheckCompileErrors Error Log: %s", c_szInfoLog);
		}
	}
	else
	{
		glGetProgramiv(uiShaderID, GL_LINK_STATUS, &iSuccess);
		if (!iSuccess)
		{
			glGetProgramInfoLog(uiShaderID, 1024, nullptr, c_szInfoLog);
			sys_err("CBaseShader::CheckCompileErrors Error Linking Program of Type %s", stShaderType.c_str());
			sys_err("CBaseShader::CheckCompileErrors Error Log: %s", c_szInfoLog);
		}
	}

	return (iSuccess);
}

std::string GetShaderName(const std::string& stShaderPath)
{
	std::string pathStr = stShaderPath;
	const size_t last_slash_idx = pathStr.find_last_of("/");
	if (std::string::npos != last_slash_idx)
	{
		pathStr.erase(0, last_slash_idx + 1);
	}

	return (pathStr);
}

std::string GetShaderTypeName(const std::string& stShaderPath)
{
	std::string stType = GetShaderName(stShaderPath);
	const size_t last_slash_idx = stType.find_last_of(".");
	if (std::string::npos != last_slash_idx)
	{
		stType.erase(0, last_slash_idx + 1);
	}

	return (stType);
}

TShaderType GetShaderType(const std::string& stShaderPath)
{
	std::string stType = GetShaderTypeName(stShaderPath);
	TShaderType shaderType;

	if (stType == "vert")
	{
		shaderType = TShaderType("vertex", GL_VERTEX_SHADER);
	}
	else if (stType == "frag")
	{
		shaderType = TShaderType("fragment", GL_FRAGMENT_SHADER);
	}
	else if (stType == "geom")
	{
		shaderType = TShaderType("geometry", GL_GEOMETRY_SHADER);
	}
	else if (stType == "comp")
	{
		shaderType = TShaderType("compute", GL_COMPUTE_SHADER);
	}
	else if (stType == "tes")
	{
		shaderType = TShaderType("tess_evaluation", GL_TESS_EVALUATION_SHADER);
	}
	else if (stType == "tcs")
	{
		shaderType = TShaderType("tess_control", GL_TESS_CONTROL_SHADER);
	}

	return (shaderType);
}
