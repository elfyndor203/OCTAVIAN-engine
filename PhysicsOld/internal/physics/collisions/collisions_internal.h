#pragma once
#include "physics/types_internal.h"

#include "cOCT_EngineStructure.h"
#include "cOCT_Communication.h"
#include "OCT_Math.h"

struct iOCT_collision {
	OCT_ID colliderA;
	OCT_ID colliderB;
	OCT_vec2 MTV;
};

extern iOCT_collision iOCT_collision_none;

iOCT_collision iOCT_physics_detectCollision(_OCT_snapshot_physics* physA, _OCT_snapshot_physics* physB);
void iOCT_physics_resolveCollision(_OCT_snapshot_physics* physA, _OCT_snapshot_physics* physB, iOCT_collision collision);