#pragma once

#include "OCT_Core.h"

#define OCT_TINT_COLOR_NONE (OCT_vec4){1.0, 1.0, 1.0, 1.0}

OCT_handle OCT_textureGroup_open(OCT_vec2 dimensions, OCT_index maxCount);
OCT_handle OCT_texture_new(OCT_handle textureGroup, const char* path);
void OCT_sprite2D_attach(OCT_handle entity, OCT_handle texture, OCT_vec4 uv, OCT_vec4 tintColor, OCT_vec2 dimensions, OCT_index drawLayer);