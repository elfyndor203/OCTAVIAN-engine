#include "systemDescription_renderer.h"

#include "OCT_Core_eng.h"

#include "renderer/renderer_int.h"
#include "renderer/sprite2D/sprite2D_int.h"
#include "renderer/camera2D/camera2D_int.h"
#include "window/window/window_int.h"
#include "window/windowSystem_int.h"


void system_register_RENDERER() {
    eOCT_componentDescription sprite2D = {
        .name = "sprite2D",
        .providedFields = eOCT_POOL_EMPTY,
        .rootAttachmentFx = iOCT_sprite2D_root,
        .stride = sizeof(iOCT_sprite2D),
        .cacheLocation = &iOCT_renderer_inst.sprite2DCache,
        .sortValueOffset = offsetof(iOCT_sprite2D, sortKey),
    };
    eOCT_componentDescription camera2D = {
        .name = "camera2D",
        .providedFields = eOCT_POOL_EMPTY,
        .rootAttachmentFx = NULL,
        .cacheLocation = &iOCT_renderer_inst.camera2DCache,
        .sortValueOffset = eOCT_POOL_SORT_NONE,
        .stride = sizeof(iOCT_camera2D)
    };
    eOCT_componentDescription components[2] = { sprite2D, camera2D };

    eOCT_fieldRequest transform2D = {
        .name = "transform2D",
        .type = eOCT_FIELDTYPE_MAT3,
        .cacheLocation = &iOCT_renderer_inst.transform2DCache,
        .optional = false,
    };
    eOCT_fieldRequest fieldRequests[1] = { transform2D };

    eOCT_systemDescription rendererSystem = {
        .name = "Renderer",
        .providedComponents = eOCT_generateComponentDescriptionPool(components, 2),
        .providedDataPools = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_generateFieldRequestPool(fieldRequests, 1),
        .initFx = system_init_RENDERER};

    iOCT_renderer_inst.systemDescription = rendererSystem;
    eOCT_registry_registerSystem(&iOCT_renderer_inst.systemDescription);
}

void system_register_WINDOW() {
    // eOCT_fieldDescription VAO = {
    //     .name = "windowVAO",
    //     .offset = offsetof(iOCT_window, VAO),
    //     .provider = eOCT_FIELDPROVIDER_DATAPOOL,
    //     .type = eOCT_FIELDTYPE_UINT64,
    // };
    // eOCT_fieldDescription windowFields[] = {VAO};
    // eOCT_dataPoolDescription window = {
    //     .name = "Window",
    //     .stride = sizeof(iOCT_window),
    //     .providedFields = eOCT_generateFieldDescriptionPool(windowFields, 1),
    //     .global = true,
    //     .cacheLocation = &iOCT_windowSystem_inst.windowCache
    // };

    eOCT_fieldDescription glfwKeyEvent = {
        .name = "glfwKeys",
        .provider = eOCT_FIELDPROVIDER_DATAPOOL,
        .offset = offsetof(iOCT_keyEvent, key),
        .type = eOCT_FIELDTYPE_INT64
    };
    eOCT_fieldDescription glfwKeyAction = {
        .name = "glfwKeyAction",
        .provider = eOCT_FIELDPROVIDER_DATAPOOL,
        .offset = 0,
        .type = eOCT_FIELDTYPE_INT64
    };
    eOCT_fieldDescription keyFields[2] = { glfwKeyEvent, glfwKeyAction };
    eOCT_dataPoolDescription glfwKeyData = {
        .name = "glfwKeyData",
        .providedFields = eOCT_generateFieldDescriptionPool(keyFields, 2),
        .global = true,
        .stride = sizeof(int),
        .cacheLocation = &iOCT_windowSystem_inst.keyCache
    };

    eOCT_dataPoolDescription dataPools[1] = { glfwKeyData };

    eOCT_systemDescription windowSystem = {
        .name = "Window",
        .providedDataPools = eOCT_generateDataPoolDescriptionPool(dataPools, 1),
        .providedComponents = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_POOL_EMPTY,
        .initFx = system_init_WINDOW
    };

    iOCT_windowSystem_inst.windowSystem = windowSystem;
    eOCT_registry_registerSystem(&iOCT_windowSystem_inst.windowSystem);
}