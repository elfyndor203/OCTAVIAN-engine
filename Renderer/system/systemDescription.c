#include "systemDescription_renderer.h"

#include "OCT_Core_eng.h"

#include "renderer/renderer_int.h"
#include "renderer/sprite2D/sprite2D_int.h"
#include "renderer/camera2D/camera2D_int.h"
#include "window/window/window_int.h"
#include "window/windowSystem_int.h"
#include "window/inputs/inputs_int.h"

void system_register_RENDERER() {
    eOCT_componentDescription sprite2D = {
        .name = "sprite2D",
        .providedFields = eOCT_POOL_EMPTY,
        .rootAttachmentFx = iOCT_sprite2D_root,
        .stride = sizeof(iOCT_sprite2D),
        .cacheLocation = &iOCT_renderer_inst.sprite2DCache,
        .sortValueOffset = offsetof(iOCT_sprite2D, sortKey),
        .entitySlotValueOffset = offsetof(iOCT_sprite2D, entityID)
    };
    eOCT_componentDescription camera2D = {
        .name = "camera2D",
        .providedFields = eOCT_POOL_EMPTY,
        .rootAttachmentFx = NULL,
        .cacheLocation = &iOCT_renderer_inst.camera2DCache,
        .sortValueOffset = eOCT_POOL_SORT_NONE,
        .stride = sizeof(iOCT_camera2D),
        .entitySlotValueOffset = offsetof(iOCT_camera2D, entityID)
    };
    eOCT_componentDescription components[2] = { sprite2D, camera2D };

    eOCT_fieldRequest transform2D = {
        .name = "globalTransform2D",
        .type = eOCT_DATATYPE_MAT3,
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
    eOCT_fieldDescription glfwKeyEvent = {
        .name = "glfwKeys",
        .providerType = eOCT_FIELDPROVIDER_EVENT,
        .offset = offsetof(iOCT_keyEvent, key),
        .type = eOCT_DATATYPE_INT64
    };
    eOCT_fieldDescription glfwKeyPress = {
        .name = "glfwKeyPress",
        .providerType = eOCT_FIELDPROVIDER_EVENT,
        .offset = offsetof(iOCT_keyEvent, pressed),
        .type = eOCT_DATATYPE_BOOL
    };
    eOCT_fieldDescription glfwKeyRelease = {
        .name = "glfwKeyRelease",
        .providerType = eOCT_FIELDPROVIDER_EVENT,
        .offset = offsetof(iOCT_keyEvent, released),
        .type = eOCT_DATATYPE_BOOL
    };
    eOCT_fieldDescription keyFields[3] = { glfwKeyEvent, glfwKeyPress, glfwKeyRelease };

    eOCT_eventDescription keyEvents = {
        .name = "keysEvents",
        .providedFields = eOCT_generateFieldDescriptionPool(keyFields, 3),
        .stride = sizeof(iOCT_keyEvent),
        .global = true,
        .cacheLocation = &iOCT_windowSystem_inst.keyEventCache,
    };

    eOCT_eventDescription events[1] = { keyEvents };

    eOCT_systemDescription windowSystem = {
        .name = "Window",
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedComponents = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_generateEventDescriptionPool(events, 1),
        .requestedFields = eOCT_POOL_EMPTY,
        .initFx = system_init_WINDOW
    };

    iOCT_windowSystem_inst.windowSystem = windowSystem;
    eOCT_registry_registerSystem(&iOCT_windowSystem_inst.windowSystem);
}