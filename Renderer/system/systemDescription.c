#include "systemDescription_renderer.h"

#include "OCT_Core_eng.h"

#include "renderer/renderer_int.h"

void system_register_RENDERER() {
    eOCT_fieldRequest transform2D = {
        .name = "transform2D",
        .type = eOCT_FIELDTYPE_MAT3,
        .optional = false
    };

    eOCT_fieldRequest fieldRequests[1] = { transform2D };

    eOCT_systemDescription rendererSystem = {
        .name = "Renderer",
        .providedComponents = eOCT_POOL_EMPTY,
        .providedDataPools = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_generateFieldRequestPool(fieldRequests, 1),
        .initFx = NULL
    };

    iOCT_renderer_inst.systemDescription = rendererSystem;
    eOCT_registry_registerSystem(&iOCT_renderer_inst.systemDescription);
}