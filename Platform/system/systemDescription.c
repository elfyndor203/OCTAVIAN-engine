
#include "OCT_Core_eng.h"

#include "../internal/platformSystem_int.h"

void system_register_PLATFORM() {
    eOCT_fieldDescription time = {
        .name = "systemTime",
        .type = eOCT_DATATYPE_DOUBLE64,
        .providerType = eOCT_FIELDPROVIDER_SINGLE,
        .offset = 0
    };
    eOCT_singleDescription timeSingle = {
        .name = "time",
        .providedField = time,
        .global = true,
        .cacheLocation = &iOCT_platformSystem_inst.timeCache
    };
    eOCT_singleDescription singles[1] = { timeSingle };

    eOCT_systemDescription platformSystem = {
        .name = "Platform",
        .providedComponents = eOCT_POOL_EMPTY,
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_POOL_EMPTY,
        .providedSingles = eOCT_generateSingleDescriptionPool(singles, 1),
        .initFx = NULL
    };

    iOCT_platformSystem_inst.systemDescription = platformSystem;
    eOCT_registry_registerSystem(&iOCT_platformSystem_inst.systemDescription);
}
