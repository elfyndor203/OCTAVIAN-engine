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

    eOCT_fieldRequest transform2D = {
        .name = "globalTransform2D",
        .type = eOCT_DATATYPE_MAT3,
        .ticketCacheLocation = &iOCT_renderer_inst.transform2DTicket,
        .optional = false,
    };

    eOCT_systemDescription rendererSystem = {
        .name = "Renderer",
        .providedComponents = eOCT_generateComponentDescriptionPool(2, sprite2D, camera2D),
        .providedDataPools = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_generateFieldRequestPool(1, transform2D),
        .initFx = system_init_RENDERER};

    iOCT_renderer_inst.systemDescription = rendererSystem;
    eOCT_registry_registerSystem(&iOCT_renderer_inst.systemDescription);
}

void system_register_WINDOW() {
    eOCT_fieldDescription glfwKeys = {
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
    eOCT_fieldDescription keyFields[3] = { glfwKeys, glfwKeyPress, glfwKeyRelease };
    eOCT_eventDescription keyEvents = {
        .name = "keysEvents",
        .providedFields = eOCT_generateFieldDescriptionPool(3, glfwKeys, glfwKeyPress, glfwKeyRelease),
        .stride = sizeof(iOCT_keyEvent),
        .global = true,
        .cacheLocation = &iOCT_windowSystem_inst.keyEventCache,
    };

    eOCT_fieldDescription mouseButton = {
        .name = "glfwMouseButton",
        .type = eOCT_DATATYPE_INT64,
        .offset = offsetof(iOCT_mouseButtonEvent, button),
        .providerType = eOCT_FIELDPROVIDER_EVENT
    };
    eOCT_fieldDescription mouseButtonPress = {
        .name = "glfwMouseButtonPress",
        .type = eOCT_DATATYPE_BOOL,
        .offset = offsetof(iOCT_mouseButtonEvent, pressed),
        .providerType = eOCT_FIELDPROVIDER_EVENT
    };
    eOCT_fieldDescription mouseButtonRelease = {
        .name = "glfwMouseButtonRelease",
        .type = eOCT_DATATYPE_BOOL,
        .offset = offsetof(iOCT_mouseButtonEvent, released),
        .providerType = eOCT_FIELDPROVIDER_EVENT
    };
    eOCT_fieldDescription mouseButtonFields[3] = { mouseButton, mouseButtonPress, mouseButtonRelease };
    eOCT_eventDescription mouseButtonEvents = {
        .name = "mouseButtonEvents",
        .providedFields = eOCT_generateFieldDescriptionPool(3, mouseButton, mouseButtonPress, mouseButtonRelease),
        .stride = sizeof(iOCT_mouseButtonEvent),
        .global = true,
        .cacheLocation = &iOCT_windowSystem_inst.mouseButtonEventCache
    };

    eOCT_fieldDescription mouseXPos = {
        .name = "glfwMouseXPos",
        .providerType = eOCT_FIELDPROVIDER_EVENT,
        .offset = offsetof(iOCT_mouseMoveEvent, xPos),
        .type = eOCT_DATATYPE_FLOAT32,
    };
    eOCT_fieldDescription mouseYPos = {
        .name = "glfwMouseYPos",
        .providerType = eOCT_FIELDPROVIDER_EVENT,
        .offset = offsetof(iOCT_mouseMoveEvent, yPos),
        .type = eOCT_DATATYPE_FLOAT32,
    };
    eOCT_eventDescription mouseMoveEvents = {
        .name = "mouseMoveEvents",
        .providedFields = eOCT_generateFieldDescriptionPool(2, mouseXPos, mouseYPos),
        .stride = sizeof(iOCT_mouseMoveEvent),
        .global = true,
        .cacheLocation = &iOCT_windowSystem_inst.mouseMoveEventCache
    };

    eOCT_fieldDescription focusedCameraMatrix = {
        .name = "focusedCameraMatrix",
        .providerType = eOCT_FIELDPROVIDER_SINGLE,
        .offset = 0,
        .type = eOCT_DATATYPE_MAT3
    };
    eOCT_singleDescription focusedCamera = {
        .name = "focusedCamera",
        .providedField = focusedCameraMatrix,
        .global = true,
        .cacheLocation = NULL
    };
    eOCT_systemDescription windowSystem = {
        .name = "Window",
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedComponents = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_generateEventDescriptionPool(3, keyEvents, mouseButtonEvents, mouseMoveEvents),
        .providedSingles = eOCT_generateSingleDescriptionPool(1, focusedCamera),
        .requestedFields = eOCT_POOL_EMPTY,
        .initFx = system_init_WINDOW
    };

    iOCT_windowSystem_inst.windowSystem = windowSystem;
    eOCT_registry_registerSystem(&iOCT_windowSystem_inst.windowSystem);
}