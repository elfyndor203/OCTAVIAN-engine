#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdbool.h>

struct iOCT_physics2D {
    OCT_local entityHandle;

    float mass;
    float inertia;
    float gravityStrength;

    OCT_vec2 velocity;
    float angVelocity;

    OCT_vec2 f_frame;
    OCT_vec2 f_const;

    OCT_vec2 prevPos;
    bool fixed;
};

void iOCT_physics2D_integrateEuler(iOCT_physics2D* physics2D, OCT_vec2* position, float* rotation, float dt);
void iOCT_physics2D_conserveL(iOCT_physics2D* center, iOCT_physics2D* target, float originalDistance, float newDistance);