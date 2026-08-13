#pragma once

#include "OCT_Core.h"

#define OCT_TINT_COLOR_NONE (OCT_vec4){1.0, 1.0, 1.0, 1.0}

OCT_global OCT_textureGroup_open(OCT_vec2 pixelDimensions, OCT_index maxCount);
OCT_global OCT_texture_new(OCT_global textureGroup, const char* path);
void OCT_sprite2D_attach(OCT_local entity, OCT_global texture, OCT_vec4 uv, OCT_vec4 tintColor, OCT_vec2 dimensions, OCT_index drawLayer);