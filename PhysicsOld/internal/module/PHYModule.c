#include "PHYModule_internal.h"
#include "physics/types_internal.h"

#include "_ECS_Output/_ECS_include.h"
#include "cOCT_Communication.h"
#include <stdio.h>

#include "physics/physicsPass/physicsPass_internal.h"
#include "physics/collisions/collisions_internal.h"

iOCT_PHYModule iOCT_PHYModule_instance = { 0 };

void OCT_PHYModule_init(OCT_vec2 gravity, unsigned int hz) {
	iOCT_PHYModule_instance.gravity = gravity;
	iOCT_PHYModule_instance.deltaT = 1.0 / hz;
}
void OCT_PHYModule_update() {
	OCT_index contextCt = _OCT_entityContext_getCount();

	OCT_ID contextID;
	OCT_index physCount;
	_OCT_snapshot_physics* snapshotPack;
	for (OCT_index i = 0; i < contextCt; i++) {
		snapshotPack = _OCT_physics2D_packSnapshot(&physCount, &contextID, i);
		//printf("Received position 1: %f %f, position 2: %f %f\n", snapshotPack[0].position.x, snapshotPack[0].position.y, snapshotPack[1].position.x, snapshotPack[1].position.y);
		iOCT_physPass_calc(snapshotPack, physCount);

		_OCT_physics2D_writeBack(physCount, contextID);
	}
}