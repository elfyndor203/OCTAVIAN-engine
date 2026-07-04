#include "systemDescription_renderer.h"

#include "OCT_Core_eng.h"

#include "../internal/renderer_int.h"
#include "../internal/sprite2D_int.h"

void system_register_RENDERER() {
    eOCT_componentDescription sprite2D = {
        .name = "sprite2D",
        .providedFields = eOCT_POOL_EMPTY,
        .rootAttachmentFx = NULL,
        .stride = sizeof(iOCT_sprite2D),
        .cacheLocation = &iOCT_renderer_inst.sprite2DCache,
        .sortValueOffset = offsetof(iOCT_sprite2D, sortKey)
    };

    eOCT_fieldRequest transform2D = {
        .name = "transform2D",
        .type = eOCT_FIELDTYPE_MAT3,
        .cacheLocation = &iOCT_renderer_inst.transform2DCache,
        .optional = false,
    };

    eOCT_componentDescription components[1] = { sprite2D };
    eOCT_fieldRequest fieldRequests[1] = { transform2D };

    eOCT_systemDescription rendererSystem = {
        .name = "Renderer",
        .providedComponents = eOCT_generateComponentDescriptionPool(components, 1),
        .providedDataPools = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_generateFieldRequestPool(fieldRequests, 1),
        .initFx = system_init_RENDERER};

    iOCT_renderer_inst.systemDescription = rendererSystem;
    eOCT_registry_registerSystem(&iOCT_renderer_inst.systemDescription);
}