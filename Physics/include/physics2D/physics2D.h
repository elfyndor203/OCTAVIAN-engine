#pragma once

#include "OCT_Core.h"

#define OCT_GRAVITY_DEFAULT ((OCT_vec2){0.0f, -1000.0f})
#define OCT_PHYSICS_REFRESH_DEFAULT 60

void OCT_physics2D_attach(OCT_local entity, float mass, bool fixed);
OCT_vec2 OCT_physics2D_setVelocity(OCT_local entity, OCT_vec2 velocity);
OCT_vec2 OCT_physics2D_addImpulse(OCT_local entity, OCT_vec2 impulse);
OCT_vec2 OCT_physics2D_addForceContinuous(OCT_local entity, OCT_vec2 force);
OCT_vec2 OCT_physics2D_addForceInstantaneous(OCT_local entity, OCT_vec2 force);
float OCT_physics2D_setGravity(OCT_local entity, float gravityStrength);
/*!
 *
 * @param entity
 * @param massOut
 * @param gravityOut
 * @param netForcesOut
 * @return velocity
 */
OCT_vec2 OCT_physics2D_read(OCT_local entity, float* massOut, float* gravityOut, OCT_vec2* netForcesOut);
/*!
 *
 * @param entity
 * @return calculated velocity from previous position
 */
OCT_vec2 OCT_physics2D_readImplicit(OCT_local entity);
