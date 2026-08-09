#include "constraints_int.h"
#include "types_int.h"

#include "physicsSystem_int.h"

OCT_local OCT_hitbox2D_new(OCT_local entity, OCT_vec2 dimensions, OCT_vec2 position, float rotation) {
    if (!eOCT_entity_hasComponentOnce(entity, iOCT_physicsSystem_inst.physics2DKey)) {
        OCT_ERROR_LOG(OCT_EXIT_REQUIREMENT_NOT_MET, "Entities must both have physics components attached");
        return OCT_LOCAL_NULL;
    }

    iOCT_hitbox2D newBox = {
        .entity = entity,
        .dimensions = dimensions,
        .position = position,
        .rotation = rotation,
        .enabled = true
    };
    eOCT_mappedPool* boxMPool = eOCT_dataPool_getLocal(iOCT_physicsSystem_inst.hitbox2DKey, entity.contextHandle);
    eOCT_mappedPool_addEntry(boxMPool, &newBox, &newBox.hitboxID, NULL);
    OCT_local newHandle = {
        .contextHandle = entity.contextHandle,
        .objectID = newBox.hitboxID
    };

    return newHandle;
}

bool iOCT_hitbox2D_solve(iOCT_hitbox2D hitboxA, iOCT_hitbox2D hitboxB) {
    if (!hitboxA.enabled || !hitboxB.enabled) {
        return false;
    }

    iOCT_physics2D* physA = eOCT_entity_getComponentOnce(hitboxA.entity, iOCT_physicsSystem_inst.physics2DKey);
    iOCT_physics2D* physB = eOCT_entity_getComponentOnce(hitboxB.entity, iOCT_physicsSystem_inst.physics2DKey);

    OCT_vec2* posA = (OCT_vec2*)eOCT_entity_getFieldOnce(hitboxA.entity, iOCT_physicsSystem_inst.position2DTicket);
    OCT_vec2* posB = (OCT_vec2*)eOCT_entity_getFieldOnce(hitboxB.entity, iOCT_physicsSystem_inst.position2DTicket);

    // Radius check
    OCT_vec2 hitboxAGlobal = OCT_vec2_add(*posA, hitboxA.position);
    OCT_vec2 hitboxBGlobal = OCT_vec2_add(*posB, hitboxB.position);
    float distance = OCT_vec2_mag(OCT_vec2_sub(hitboxAGlobal, hitboxBGlobal));
    float radiusA = OCT_vec2_mag(OCT_vec2_div(hitboxA.dimensions, 2));
    float radiusB = OCT_vec2_mag(OCT_vec2_div(hitboxB.dimensions, 2));
    if (distance > radiusA + radiusB) {
        return false;
    }

    // SAT check
    OCT_rect2 globalRectA = {
        .dimensions = hitboxA.dimensions,
        .center = OCT_vec2_add(hitboxA.position, *posA),
        .rotationDeg = hitboxA.rotation
    };
    OCT_rect2 globalRectB = {
        .dimensions = hitboxB.dimensions,
        .center = OCT_vec2_add(hitboxB.position, *posB),
        .rotationDeg = hitboxB.rotation
    };
    OCT_vec2 sourceAxis;
    float overlap;
    OCT_AorB sourceRect;
    OCT_vec2 MTV = OCT_rect2_SAT(globalRectA, globalRectB, &sourceAxis, &overlap, &sourceRect);

    if (OCT_vec2_equal(MTV, OCT_VEC2_ZERO, OCT_FLOAT_EPSILON)) {
        return false;
    }

    OCT_vec2 midpoint = OCT_vec2_div(OCT_vec2_add(globalRectA.center, globalRectB.center), 2.0f);
    // OCT_vec2 contactPoint =

}
