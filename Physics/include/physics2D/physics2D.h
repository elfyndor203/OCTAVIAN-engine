#pragma once

#include "OCT_Core.h"

#define OCT_GRAVITY_DEFAULT ((OCT_vec2){0.0f, -1000.0f})
#define OCT_PHYSICS_REFRESH_DEFAULT 60

void OCT_physics2D_attach(OCT_handle entity, float mass, bool fixed);
OCT_vec2 OCT_physics2D_setVelocity(OCT_handle entity, OCT_vec2 velocity);
OCT_vec2 OCT_physics2D_addImpulse(OCT_handle entity, OCT_vec2 impulse);
OCT_vec2 OCT_physics2D_addForceContinuous(OCT_handle entity, OCT_vec2 force);
OCT_vec2 OCT_physics2D_addForceInstantaneous(OCT_handle entity, OCT_vec2 force);
float OCT_physics2D_setGravity(OCT_handle entity, float gravityStrength);
OCT_vec2 OCT_physics2D_read(OCT_handle entity, float* massOut, float* gravityOut, OCT_vec2* netForcesOut);