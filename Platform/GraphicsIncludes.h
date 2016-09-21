#pragma once
#ifndef GRAPHICS_INCLUDES_H
#define GRAPHICS_INCLUDES_H

// Defines the files needed to support OpenGL

#define GLEW_NO_GLU
#include <gl/glew.h>
#include <gl/wglew.h>

#pragma comment( lib, "OpenGL32.lib" )
#pragma comment( lib, "glew32.lib" )

#endif