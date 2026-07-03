#pragma once
#include "renderer/types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>

#include "sprite2D_int.h"

struct iOCT_spriteFullData {
    iOCT_spriteData spriteData;
    OCT_mat3 transform;
};
struct iOCT_renderer {
    eOCT_systemDescription systemDescription;
    eOCT_fieldRequest transform2DCache;
    eOCT_componentDescription sprite2DCache;

    eOCT_IDMap textureGroupMap;
    eOCT_pool textureGroupPool;

    eOCT_pool layerCountsTotalsPool;
    eOCT_pool drawOrderPool;

    eOCT_pool spriteFullDataBuffer;
    GLuint spriteVBO;
    GLuint spriteVAO;
    GLuint spriteEBO;
};

extern iOCT_renderer iOCT_renderer_inst;

void iOCT_renderer_init();

void iOCT_renderer_drawLayer();