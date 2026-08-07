#include "OCT_Core_eng.h"

#include "physics2D/physics2D_int.h"
#include "physicsSystem_int.h"

void system_register_PHYSICS() {
    eOCT_componentDescription physics2D = {
        .name = "physics2D",
        .entityHandleValueOffset = offsetof(iOCT_physics2D, entityHandle),
        .providedFields = eOCT_POOL_EMPTY,
        .sort = false,
        .sortValueOffset = eOCT_POOL_SORT_NONE,
        .stride = sizeof(iOCT_physics2D),
        .rootAttachmentFx = NULL,
        .keyCacheLocation = &iOCT_physicsSystem_inst.physics2DKey
    };

    eOCT_fieldRequest transform2D = {
        .name = "globalTransform2D",
        .optional = false,
        .ticketCache = &iOCT_physicsSystem_inst.transform2DTicket,
        .type = eOCT_DATATYPE_MAT3,
    };
    eOCT_fieldRequest position2D = {
        .name = "position",
        .optional = false,
        .ticketCache = &iOCT_physicsSystem_inst.position2DTicket,
        .type = eOCT_DATATYPE_VEC2,
    };

    eOCT_systemDescription physicsSystem = {
        .name = "Physics",
        .providedComponents = eOCT_generateComponentDescriptionPool(1, physics2D),
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_POOL_EMPTY,
        .providedSingles = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_generateFieldRequestPool(2, transform2D, position2D),
        .initFx = iOCT_physicsSystem_init
    };

    iOCT_physicsSystem_inst.systemID = eOCT_registry_registerSystem(physicsSystem);
}