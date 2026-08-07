#include "constraints_int.h"

void OCT_constrain_rope2D(OCT_handle entityA, OCT_handle entityB, float length) {
    if (!eOCT_entity_hasComponentOnce(entityA, iOCT_physicsSystem_inst.physics2DKey) || !eOCT_entity_hasComponentOnce(entityB, iOCT_physicsSystem_inst.physics2DKey)) {
        OCT_ERROR_LOG(OCT_EXIT_REQUIREMENT_NOT_MET, "Entities must both have physics components attached");
        return;
    }

    iOCT_constraint_rope2D rope = {
        .entityA = entityA,
        .entityB = entityB,
        .length = length
    };
    eOCT_pool_addEntryNew(&iOCT_physicsSystem_inst.ropeConstraints, &rope, NULL);
}

void iOCT_constraintSolve_rope(iOCT_constraint_rope2D constraint, eOCT_contextToken contextToken) {
    // iOCT_physics2D* physA = eOCT_entity_getComponent(contextToken, constraint.entityA, iOCT_physicsSystem_inst.physics2DKey);
    // iOCT_physics2D* physB = eOCT_entity_getComponent(contextToken, constraint.entityB, iOCT_physicsSystem_inst.physics2DKey);
    iOCT_physics2D* physA = eOCT_entity_getComponentOnce(constraint.entityA, iOCT_physicsSystem_inst.physics2DKey);
    iOCT_physics2D* physB = eOCT_entity_getComponentOnce(constraint.entityB, iOCT_physicsSystem_inst.physics2DKey);

    OCT_vec2* posA = (OCT_vec2*)eOCT_entity_getField(contextToken, constraint.entityA, iOCT_physicsSystem_inst.position2DTicket);
    OCT_vec2* posB = (OCT_vec2*)eOCT_entity_getField(contextToken, constraint.entityB, iOCT_physicsSystem_inst.position2DTicket);

    float distance = OCT_vec2_mag(OCT_vec2_sub(*posB, *posA));

    if (distance < constraint.length) {
        return;
    }

    if (physA->fixed && physB->fixed) {
        return;
    }

    if (!physA->fixed && !physB->fixed) {
        OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Two unfixed rope constraint not yet implemented");
    }

    OCT_vec2* toMovePos;
    OCT_vec2* centerPos;
    iOCT_physics2D* toMovePhys;
    iOCT_physics2D* centerPhys;
    if (physA->fixed) {
        centerPos = posA;
        centerPhys = physA;
        toMovePos = posB;
        toMovePhys = physB;
    }
    else {
        centerPos = posB;
        centerPhys = physB;
        toMovePos = posA;
        toMovePhys = physA;
    }

    OCT_vec2 fromCenter = OCT_vec2_sub(*toMovePos, *centerPos);
    OCT_vec2 fromCenterUnit = OCT_vec2_unit(fromCenter);
    OCT_vec2 correctedPos = OCT_vec2_add(OCT_vec2_mul(fromCenterUnit, constraint.length), *centerPos);

    float radialSpeed = OCT_vec2_dot(toMovePhys->v_lin, fromCenterUnit);

    if (radialSpeed > 0) {
        OCT_vec2 correctionVel = OCT_vec2_mul(fromCenterUnit, radialSpeed);
        toMovePhys->v_lin = OCT_vec2_sub(toMovePhys->v_lin, correctionVel);
    }
    *toMovePos = correctedPos;
}