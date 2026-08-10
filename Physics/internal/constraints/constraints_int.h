#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

#include "physics2D/physics2D_int.h"
#include "physicsSystem_int.h"

struct iOCT_rope2D {
    OCT_ID ropeID;

    OCT_local entityA;
    OCT_local entityB;

    float length;

    bool enabled;
};

struct iOCT_hitbox2D {
    OCT_ID hitboxID;

    OCT_local entity;

    OCT_vec2 dimensions;
    OCT_vec2 position;
    float rotation;

    bool enabled;
};

void iOCT_rope2D_solve(iOCT_rope2D rope, eOCT_contextToken contextToken);
bool iOCT_hitbox2D_solve(iOCT_hitbox2D hitboxA, iOCT_hitbox2D hitboxB);