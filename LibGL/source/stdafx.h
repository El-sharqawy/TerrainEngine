// Stdafx.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include "glad.h"
#include "stb_image.h"
#include "stb_image_write.h"

#include "../../LibMath/source/stdafx.h"

#include "utils.h"

#include "camera.h"
#include "texture.h"
#include "../../LibMath/source/grid.h"
#include "mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#define DEFAULT_WINDOW_WIDTH 1600
#define DEFAULT_WINDOW_HEIGHT 960



// add headers that you want to pre-compile here

