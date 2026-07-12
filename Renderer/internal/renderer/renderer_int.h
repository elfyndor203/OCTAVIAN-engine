#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>

#include "sprite2D/sprite2D_int.h"

#define GL_CHECK() { GLenum err = glGetError(); if (err != GL_NO_ERROR) printf("GL error %d at line %d\n", err, __LINE__); }
#define iOCT_SYSTEMTEX_DIMENSIONS ((OCT_vec2){100, 100})

struct iOCT_spriteFullData {
    iOCT_spriteData spriteData;
    OCT_mat3 transform;
};
struct iOCT_renderer {
    // system information
    eOCT_systemDescription systemDescription;
    eOCT_fieldRequest transform2DCache;
    eOCT_fieldRequest windowVAOCache;
    eOCT_componentDescription sprite2DCache;
    eOCT_componentDescription camera2DCache;

    // texture storage
    eOCT_IDMap textureGroupMap;
    eOCT_pool textureGroupPool;

    // openGL sprite drawing
    eOCT_pool spriteFullDataBuffer;
    GLuint spriteVAO;
    GLuint quadVBO;
    GLuint quadEBO;
    GLuint spriteDataVBO;
    OCT_index spriteDataVBOCapacity;
    GLuint spriteShaderProgram;

    // provided at init
    OCT_handle gizmoTex;

    // uniforms
    GLint cameraUniform;
};

extern iOCT_renderer iOCT_renderer_inst;

void system_init_RENDERER();

void iOCT_renderer_drawLayer();
void iOCT_setupNewSpriteVAO(GLuint VAO);
