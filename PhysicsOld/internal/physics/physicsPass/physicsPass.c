
#include "_ECS_Output/_ECS_include.h"
#include "OCT_Math.h"
#include <stdio.h>

#include "physics/dynamics/dynamics_internal.h"
#include "physics/collisions/collisions_internal.h"

void iOCT_physPass_calc(_OCT_snapshot_physics* snapshotPack, OCT_index count) {
	_OCT_snapshot_physics* physA;
	_OCT_snapshot_physics* physB;
	iOCT_collision collision;
	OCT_index physCt = 0;

	for (physCt = 0; physCt < count; physCt++) {
		physA = &snapshotPack[physCt];
		iOCT_physPass_addGravity(physA);
		iOCT_physPass_integrateVelocity(physA);
	}
	for (physCt = 0; physCt < count; physCt++) {
		physA = &snapshotPack[physCt];
		for (OCT_index collisCt = physCt + 1; collisCt < count; collisCt++) {
			physB = &snapshotPack[collisCt];
			collision = iOCT_physics_detectCollision(physA, physB);
			if (OCT_vec2_mag(collision.MTV) > 0) {
				iOCT_physics_resolveCollision(physA, physB, collision);
			}
		}
	}
	for (physCt = 0; physCt < count; physCt++) {
		physA = &snapshotPack[physCt];
		iOCT_physPass_integratePosition(physA);
		iOCT_physPass_finish(physA);
	}
}