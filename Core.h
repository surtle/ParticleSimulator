////////////////////////////////////////
// Core.h
////////////////////////////////////////

#pragma once

// This file just has a bunch of common stuff used by all objects. It mainly just
// includes GL and some other standard headers.

// Set up GLM
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

// Set up GLEW
//#define GLEW_STATIC
//#pragma comment(lib, "glew32.lib")
#include "GL/glew.h"

//#include "..\packages\nupengl.core.0.1.0.1\build\native\include\GL\glew.h" 
#include "packages/freeglut.2.8.1.15/build/native/include/GL/freeglut.h" 

// Set up GLUT
#include "GL/glut.h"

// Add AntTweakBar
#include <AntTweakBar.h>

// Types
typedef unsigned int uint;

// STD library
#include <vector>
#include <ctype.h>

#define ROTX 30
#define ROTY 31
#define ROTZ 32
