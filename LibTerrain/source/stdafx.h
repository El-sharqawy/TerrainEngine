// stdafx.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef STDAFX_H
#define STDAFX_H

#include "../../LibMath/source/stdafx.h"
#include "../../LibGL/source/stdafx.h"
#include "../../LibGL/source/window.h"
#include "../../LibGame/source/CommonDefines.h"

#include <cmath>

#undef min
#undef max
#undef minmax

// add headers that you want to pre-compile here
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define BUFFER_OFFSET(offset) ((const void *)(offset))

// Terrain Shader
#define COLOR_TEXTURE_UNIT_0 GL_TEXTURE0
#define COLOR_TEXTURE_UNIT_INDEX_0 0
#define COLOR_TEXTURE_UNIT_1 GL_TEXTURE1
#define COLOR_TEXTURE_UNIT_INDEX_1 1
#define COLOR_TEXTURE_UNIT_2 GL_TEXTURE2
#define COLOR_TEXTURE_UNIT_INDEX_2 2
#define COLOR_TEXTURE_UNIT_3 GL_TEXTURE3
#define COLOR_TEXTURE_UNIT_INDEX_3 3
#define COLOR_TEXTURE_UNIT_4 GL_TEXTURE4
#define COLOR_TEXTURE_UNIT_INDEX_4 4
#define COLOR_TEXTURE_UNIT_5 GL_TEXTURE5
#define COLOR_TEXTURE_UNIT_INDEX_5 5
#define COLOR_TEXTURE_UNIT_6 GL_TEXTURE6
#define COLOR_TEXTURE_UNIT_INDEX_6 6
#define COLOR_TEXTURE_UNIT_7 GL_TEXTURE7
#define COLOR_TEXTURE_UNIT_INDEX_7 7
#define COLOR_TEXTURE_UNIT_8 GL_TEXTURE8
#define COLOR_TEXTURE_UNIT_INDEX_8 8
#define COLOR_TEXTURE_UNIT_9 GL_TEXTURE9
#define COLOR_TEXTURE_UNIT_INDEX_9 9
#define COLOR_TEXTURE_UNIT_10 GL_TEXTURE10
#define COLOR_TEXTURE_UNIT_INDEX_10 10

// Water shader
#define REFLECTION_TEXTURE_UNIT       GL_TEXTURE4
#define REFLECTION_TEXTURE_UNIT_INDEX 4
#define REFRACTION_TEXTURE_UNIT       GL_TEXTURE5
#define REFRACTION_TEXTURE_UNIT_INDEX 5
#define DUDV_TEXTURE_UNIT             GL_TEXTURE6
#define DUDV_TEXTURE_UNIT_INDEX       6
#define NORMAL_MAP_TEXTURE_UNIT       GL_TEXTURE7
#define NORMAL_MAP_TEXTURE_UNIT_INDEX 7
#define DEPTH_MAP_TEXTURE_UNIT        GL_TEXTURE8
#define DEPTH_MAP_TEXTURE_UNIT_INDEX  8

#define SKYBOX_TEXTURE_UNIT       GL_TEXTURE9
#define SKYBOX_TEXTURE_UNIT_INDEX 9
#define SKYBOX_DEPTH_TEXTURE_UNIT       GL_TEXTURE10
#define SKYBOX_DEPTH_TEXTURE_UNIT_INDEX 10

inline GLint cast_intf(float fNum)
{
	return (static_cast<GLint>(std::round(fNum)));
}

inline GLfloat cast_floati(GLint fNum)
{
	return (static_cast<GLfloat>(fNum));
}

#endif //STDAFX_H
