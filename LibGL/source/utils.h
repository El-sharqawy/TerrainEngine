#pragma once

#include "../../LibMath/source/stdafx.h"
#include <assimp/types.h>

#if defined(_WIN64)
#define sys_err(...) fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\n")
#define sys_log(...) fprintf(stdout, __VA_ARGS__), fprintf(stdout, "\n")
#else
#define sys_err(...) fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\n")
#define sys_log(...) fprintf(stdout, __VA_ARGS__), fprintf(stdout, "\n")
#endif

//int GetGLMajorVersion();
//int GetGLMinorVersion();

extern bool IsGLVersionHigher(int MajorVer, int MinorVer);
extern std::string GetDirFromFilename(const std::string& Filename);

extern char* ReadBinaryFile(const char* pFilename, int& size);
extern void WriteBinaryFile(const char* pFilename, const void* pData, int size);

extern std::string GetFullPath(const std::string& Dir, const aiString& Path);

#define COLOR_TEXTURE_UNIT              GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX        0
#define SHADOW_TEXTURE_UNIT             GL_TEXTURE1
#define SHADOW_TEXTURE_UNIT_INDEX       1
#define NORMAL_TEXTURE_UNIT             GL_TEXTURE2
#define NORMAL_TEXTURE_UNIT_INDEX       2
#define RANDOM_TEXTURE_UNIT             GL_TEXTURE3
#define RANDOM_TEXTURE_UNIT_INDEX       3
#define DISPLACEMENT_TEXTURE_UNIT       GL_TEXTURE4
#define DISPLACEMENT_TEXTURE_UNIT_INDEX 4
#define ALBEDO_TEXTURE_UNIT             GL_TEXTURE5
#define ALBEDO_TEXTURE_UNIT_INDEX       5          
#define ROUGHNESS_TEXTURE_UNIT          GL_TEXTURE6
#define ROUGHNESS_TEXTURE_UNIT_INDEX    6
#define MOTION_TEXTURE_UNIT             GL_TEXTURE7
#define MOTION_TEXTURE_UNIT_INDEX       7
#define SPECULAR_EXPONENT_UNIT             GL_TEXTURE8
#define SPECULAR_EXPONENT_UNIT_INDEX       8
#define CASCACDE_SHADOW_TEXTURE_UNIT0       SHADOW_TEXTURE_UNIT
#define CASCACDE_SHADOW_TEXTURE_UNIT0_INDEX SHADOW_TEXTURE_UNIT_INDEX
#define CASCACDE_SHADOW_TEXTURE_UNIT1       GL_TEXTURE9
#define CASCACDE_SHADOW_TEXTURE_UNIT1_INDEX 9
#define CASCACDE_SHADOW_TEXTURE_UNIT2       GL_TEXTURE10
#define CASCACDE_SHADOW_TEXTURE_UNIT2_INDEX 10
#define SHADOW_CUBE_MAP_TEXTURE_UNIT        GL_TEXTURE11
#define SHADOW_CUBE_MAP_TEXTURE_UNIT_INDEX  11
#define SHADOW_MAP_RANDOM_OFFSET_TEXTURE_UNIT       GL_TEXTURE12
#define SHADOW_MAP_RANDOM_OFFSET_TEXTURE_UNIT_INDEX 12
#define DETAIL_MAP_TEXTURE_UNIT                     GL_TEXTURE13
#define DETAIL_MAP_TEXTURE_UNIT_INDEX               13
#define METALLIC_TEXTURE_UNIT                       GL_TEXTURE14
#define METALLIC_TEXTURE_UNIT_INDEX                 14
#define HEIGHT_TEXTURE_UNIT                         GL_TEXTURE15
#define HEIGHT_TEXTURE_UNIT_INDEX                   15
