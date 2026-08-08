#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

#include "physics2D/physics2D_int.h"
#include "physicsSystem_int.h"

struct iOCT_constraint_rope2D {
    OCT_ID constraintID;

    OCT_handle entityA;
    OCT_handle entityB;

    float length;
};

void iOCT_constraintSolve_rope(iOCT_constraint_rope2D constraint, eOCT_contextToken contextToken);