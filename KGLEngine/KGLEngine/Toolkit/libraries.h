// Developed by Kelin Lyu.
#ifndef libraries_h
#define libraries_h
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <dirent.h>
#include <unistd.h>
#define strcpy_s strcpy
#endif
#include <limits.h>
#include <stdio.h>
#include <ctype.h>
#include <utility>
#include <functional>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "glm/gtx/string_cast.hpp"
#include <GLFW/glfw3.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "assimp_helper.h"
#include "glm_helper.h"
#include "keys.h"
using namespace std;
using namespace glm;
#endif
