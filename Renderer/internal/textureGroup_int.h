#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>

struct iOCT_textureGroup {
    OCT_ID textureGroupID;

    GLuint glTexArray;
    OCT_vec2 dimensions;

    eOCT_IDMap textureMap; // maps to texture array GLsizei
    OCT_index textureCount;
};

void iOCT_renderer_addToLayer(iOCT_textureGroup* texGroup, OCT_index newSpriteLayer);