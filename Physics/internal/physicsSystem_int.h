#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_physicsSystem {
    OCT_ID systemID;

    eOCT_componentKey physics2DKey;
    eOCT_dataPoolKey rope2DKey;
    eOCT_dataPoolKey hitbox2DKey;
    eOCT_fieldTicket transform2DTicket;
    eOCT_fieldTicket position2DTicket;
    eOCT_fieldTicket rotationTicket;

    eOCT_singleKey box2DWorldKey;

    OCT_vec2 worldGravity;
    double dt;
    OCT_index constraintSolveIterations;

    float unitsPerB2Meter;
};

extern iOCT_physicsSystem iOCT_physicsSystem_inst;

void iOCT_physicsSystem_init();
void iOCT_physicsSystem_contextSetup(OCT_global context);