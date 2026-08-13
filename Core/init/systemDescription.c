#include "OCT_Core_eng.h"

#include "ECS/ECS_int.h"
#include "ECs/entity_int.h"

void init_system_register_CORE() {
    eOCT_componentDescription entityMeta = {
        .name = "entityMeta",
        .entityHandleValueOffset = offsetof(iOCT_entityMeta, entity),
        .keyCacheLocation = &iOCT_ECS_inst.entityMetaKey,
        .providedFields = eOCT_POOL_EMPTY,
        .rootAttachmentFx = NULL,   // done inside of entityContext creation already __NOTE__ unsure
        .sort = false,
        .stride = sizeof(iOCT_entityMeta)
    };

    eOCT_systemDescription systemDescription = {
        .name = "OCTAVIANEngine CORE",
        .contextInitFx = NULL,
        .initFx = NULL,
        .providedComponents = eOCT_generateComponentDescriptionPool(1, entityMeta),
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_POOL_EMPTY,
        .providedSingles = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_POOL_EMPTY,
    };

    eOCT_registry_registerSystem(systemDescription);
}