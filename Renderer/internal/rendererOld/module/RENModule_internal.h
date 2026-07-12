#pragma once
#include "cOCT_EngineStructure.h"
#include "renderer/types_internal.h"

#include <glad/glad.h>

#define GL_CHECK() { GLenum err = glGetError(); if (err != GL_NO_ERROR) printf("GL error %d at line %d\n", err, __LINE__); }
#define iOCT_TEXTURE_INITIALCAPACITY 16

struct iOCT_RENModule {
	cOCT_IDMap layerMap;
	cOCT_pool layerPool;

	GLuint spriteVBO;
	GLuint spriteEBO;
	GLuint spriteShader;

	size_t textureCapacity;
	GLuint* textureList;

	OCT_vec2 worldScale;
	GLuint worldProjUniform;
	OCT_mat3 worldProj;
	OCT_mat3 inverseWorldProj;

	GLint cameraProjUniform;
};
extern iOCT_RENModule iOCT_RENModule_instance;

void iOCT_RENModule_init(OCT_vec2 resolution);

void iOCT_RENModule_update();
void iOCT_RENModule_handleMSGs();