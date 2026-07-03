#pragma once
#include "OCT_Core.h"

OCT_handle OCT_textureGroup_open(OCT_vec2 dimensions, OCT_index maxCount);
OCT_handle OCT_texture_new(OCT_handle textureGroup, const char* path);