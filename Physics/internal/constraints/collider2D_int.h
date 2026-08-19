#pragma once

#include "OCT_Core_eng.h"
#include <box2d/box2d.h>

struct iOCT_collider2D {
    OCT_ID colliderID;

    OCT_shapeType shape;
    OCT_vec2 origin;
    OCT_vec2 dimensions;
    float rotation;

    b2ShapeId b2ShapeID;
};

