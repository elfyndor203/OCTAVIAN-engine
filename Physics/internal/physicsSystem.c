#include "physicsSystem_int.h"

#include "physics2D/physics2D.h"
#include "physics2D/physics2D_int.h"
#include "constraints/constraints_int.h"

#define iOCT_PHYSICS_CONSTRAINT_SOLVE_ITERATIONS_DEFAULT 10

iOCT_physicsSystem iOCT_physicsSystem_inst = {0};

void iOCT_physicsSystem_init() {
    iOCT_physicsSystem_inst.worldGravity = OCT_GRAVITY_DEFAULT;
    iOCT_physicsSystem_inst.dt = 1.0 / OCT_PHYSICS_REFRESH_DEFAULT;

    iOCT_physicsSystem_inst.constraintSolveIterations = iOCT_PHYSICS_CONSTRAINT_SOLVE_ITERATIONS_DEFAULT;
    iOCT_physicsSystem_inst.ropeConstraints = eOCT_pool_open(iOCT_physicsSystem_inst.systemID, eOCT_POOL_CAPACITY_DEFAULT, sizeof(iOCT_constraint_rope2D));
}

void eOCT_PHYSICS_update(OCT_handle context) {
    eOCT_pool* physicsPool = eOCT_component_getPool(context, iOCT_physicsSystem_inst.physics2DKey);
    iOCT_physics2D* physicsArray = (iOCT_physics2D*)physicsPool->array;
    eOCT_contextToken contextToken = eOCT_context_getToken(context);
    for (OCT_index physCtr = 0; physCtr < physicsPool->count; physCtr++) {
        iOCT_physics2D* physics = &physicsArray[physCtr];

        if (physics->fixed) {
            continue;
        }
        OCT_vec2* position = (OCT_vec2*)eOCT_entity_getField(contextToken, physics->entityHandle, iOCT_physicsSystem_inst.position2DTicket);
        physics->prevPos = *position;
        iOCT_physics2D_integrateEuler(physics, position, iOCT_physicsSystem_inst.dt);
    }

    eOCT_pool* ropePool = &iOCT_physicsSystem_inst.ropeConstraints;
    iOCT_constraint_rope2D* ropeArray = (iOCT_constraint_rope2D*)ropePool->array;
    for (OCT_index iteration = 0; iteration < iOCT_physicsSystem_inst.constraintSolveIterations; iteration++) {
        for (OCT_index ropeCtr = 0; ropeCtr < ropePool->count; ropeCtr++) {
            iOCT_constraint_rope2D rope = ropeArray[ropeCtr];
            iOCT_constraintSolve_rope(rope, contextToken);
        }
    }

    for (OCT_index physCtr = 0; physCtr < physicsPool->count; physCtr++) {
        iOCT_physics2D* physics = &physicsArray[physCtr];
        OCT_vec2 position = *(OCT_vec2*)eOCT_entity_getFieldOnce(physics->entityHandle, iOCT_physicsSystem_inst.position2DTicket);
        OCT_vec2 frameDelta = OCT_vec2_sub(position, physics->prevPos);
        physics->v_lin = OCT_vec2_div(frameDelta, iOCT_physicsSystem_inst.dt);
    }
}