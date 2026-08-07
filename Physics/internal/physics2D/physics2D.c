#include "physics2D_int.h"
#include "types_int.h"
#include "physics2D/physics2D.h"

#include "OCT_Core_eng.h"

#include "physicsSystem_int.h"

static OCT_vec2 iOCT_physics2D_resolveFrameNetForce(iOCT_physics2D* physics2D);

void OCT_physics2D_attach(OCT_handle entity, float mass, bool fixed) {
    if (mass <= 0) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Entities must have positive mass. Fix in place or adjust gravity instead.");
    }
    iOCT_physics2D newPhysics = {
        .entityHandle = entity,
        .mass = mass,
        .gravityStrength = 1,
        .f_const = OCT_vec2_zero,   // does not include gravity
        .f_frame = OCT_vec2_zero,
        .v_lin = OCT_vec2_zero,
        .prevPos = OCT_vec2_zero,
        .fixed = fixed
    };
    eOCT_entity_attachComponentOnce(entity, iOCT_physicsSystem_inst.physics2DKey, &newPhysics, NULL);

    printf("Attached physics2D to entity %zu\n", entity.objectID);
}

OCT_vec2 OCT_physics2D_setVelocity(OCT_handle entity, OCT_vec2 velocity) {
    iOCT_physics2D* physics = eOCT_entity_getComponentOnce(entity, iOCT_physicsSystem_inst.physics2DKey);

    OCT_vec2 oldVelocity = physics->v_lin;
    physics->v_lin = velocity;
    return OCT_vec2_sub(velocity, oldVelocity);
}

OCT_vec2 OCT_physics2D_addImpulse(OCT_handle entity, OCT_vec2 impulse) {
    iOCT_physics2D* physics = eOCT_entity_getComponentOnce(entity, iOCT_physicsSystem_inst.physics2DKey);

    physics->v_lin = OCT_vec2_add(physics->v_lin, OCT_vec2_div(impulse, physics->mass));
    return physics->v_lin;
}

OCT_vec2 OCT_physics2D_addForceContinuous(OCT_handle entity, OCT_vec2 force) {
    iOCT_physics2D* physics = eOCT_entity_getComponentOnce(entity, iOCT_physicsSystem_inst.physics2DKey);

    physics->f_const = OCT_vec2_add(physics->f_const, force);
    return physics->f_const;
}

OCT_vec2 OCT_physics2D_addForceInstantaneous(OCT_handle entity, OCT_vec2 force) {
    iOCT_physics2D* physics = eOCT_entity_getComponentOnce(entity, iOCT_physicsSystem_inst.physics2DKey);

    physics->f_frame = OCT_vec2_add(physics->f_frame, force);
    return physics->f_frame;
}

float OCT_physics2D_setGravity(OCT_handle entity, float gravityStrength) {
    iOCT_physics2D* physics = eOCT_entity_getComponentOnce(entity, iOCT_physicsSystem_inst.physics2DKey);

    float oldGravity = physics->gravityStrength;
    physics->gravityStrength = gravityStrength;
    return gravityStrength - oldGravity;
}

OCT_vec2 OCT_physics2D_read(OCT_handle entity, float* massOut, float* gravityOut, OCT_vec2* netForcesOut) {
    iOCT_physics2D* physics = eOCT_entity_getComponentOnce(entity, iOCT_physicsSystem_inst.physics2DKey);

    if (massOut) {
        *massOut = physics->mass;
    }
    if (gravityOut) {
        *gravityOut = physics->gravityStrength;
    }
    if (netForcesOut) {
        *netForcesOut = OCT_vec2_add(OCT_vec2_add(physics->f_const, physics->f_frame), OCT_vec2_mul(iOCT_physicsSystem_inst.worldGravity, physics->gravityStrength));
    }
    return physics->v_lin;
}

void iOCT_physics2D_integrateEuler(iOCT_physics2D* physics2D, OCT_vec2* position, float dt) {
    OCT_vec2 fNet = iOCT_physics2D_resolveFrameNetForce(physics2D);
    OCT_vec2 accel_lin = OCT_vec2_div(fNet, physics2D->mass);

    OCT_vec2 deltaVel_lin = OCT_vec2_mul(accel_lin, dt);
    OCT_vec2 newVel_lin = OCT_vec2_add(deltaVel_lin, physics2D->v_lin);
    OCT_vec2 deltaPos = OCT_vec2_mul(newVel_lin, dt);

    *position = OCT_vec2_add(*position, deltaPos);
    physics2D->v_lin = newVel_lin;

    // printf("New velocity: %f, %f\n", physics2D->v_lin.x, physics2D->v_lin.y);
}

static OCT_vec2 iOCT_physics2D_resolveFrameNetForce(iOCT_physics2D* physics2D) {
    OCT_vec2 gravity = OCT_vec2_mul(iOCT_physicsSystem_inst.worldGravity, physics2D->gravityStrength * physics2D->mass);
    OCT_vec2 constWGravity = OCT_vec2_add(gravity, physics2D->f_const);
    OCT_vec2 fNet = OCT_vec2_add(constWGravity, physics2D->f_frame);

    physics2D->f_frame = OCT_vec2_zero;

    return fNet;
}
