#pragma once
#include "OCT_Core.h"

OCT_global OCT_textureGroup_open(OCT_vec2 pixelDimensions, OCT_index maxCount);
OCT_global OCT_texture_new(OCT_global textureGroup, const char* path);