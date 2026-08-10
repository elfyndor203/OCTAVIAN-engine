#pragma once
#include <glad/glad.h>

#define iOCT_SHADER_VERT 1
#define iOCT_SHADER_FRAG 2

GLuint iOCT_shader_load(char* path, int type);
GLuint iOCT_shader_createProgram(char* vertPath, char* fragPath);