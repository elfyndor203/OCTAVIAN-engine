
#include "_ECS_Output/_ECS_include.h"
#include "OCT_EngineStructure.h"
#include "OCT_Math.h"
#include <stdio.h>

#include "module/PHYModule_internal.h"

void iOCT_physPass_addGravity(_OCT_snapshot_physics* snapshot) {
	snapshot->forceNet = OCT_vec2_add(snapshot->forceNet, OCT_vec2_mul(iOCT_PHYModule_instance.gravity, snapshot->gravity));
}

void iOCT_physPass_integrateVelocity(_OCT_snapshot_physics* snapshot) {
	OCT_vec2 lin_a;
	float ang_a;

	lin_a = OCT_vec2_div(snapshot->forceNet, snapshot->mass / iOCT_PHYModule_instance.deltaT);	// get acceleration
	ang_a = (snapshot->torqueNet / snapshot->inertia) * iOCT_PHYModule_instance.deltaT;

	snapshot->lin_v = OCT_vec2_add(snapshot->lin_v, lin_a);		// apply to velocity
	snapshot->ang_v += ang_a;
}

void iOCT_physPass_integratePosition(_OCT_snapshot_physics* snapshot) {
	snapshot->position = OCT_vec2_add(snapshot->position, snapshot->lin_v);	// update position
	snapshot->rotation += snapshot->ang_v;
}

void iOCT_physPass_finish(_OCT_snapshot_physics* snapshot) {
	snapshot->forceNet = OCT_vec2_zero;
	snapshot->torqueNet = 0;
}