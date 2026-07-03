#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_spriteData {
    OCT_vec4 uv;
    OCT_vec4 color;
    OCT_vec2 dimensions;
    OCT_mat3 transform;
};

struct iOCT_sprite2D {
    OCT_handle entity;
    OCT_ID texGroupID;
    OCT_index texArrayLayer;
    OCT_index drawLayer;

    iOCT_spriteData spriteData;
};



void OCT_sprite2D_attach(OCT_handle entity, OCT_handle texture, OCT_vec4 uv, OCT_vec4 color, OCT_index drawLayer);