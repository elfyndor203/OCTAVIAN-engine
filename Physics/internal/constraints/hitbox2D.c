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