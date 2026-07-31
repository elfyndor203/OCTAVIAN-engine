
#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

void system_register_INPUT() {
    eOCT_fieldRequest key = {
        .name = "glfwKeys",
        .type = eOCT_DATATYPE_INT64,
        .optional = false,
        .ticketCacheLocation = &iOCT_inputSystem_inst.keyCache
    };
    eOCT_fieldRequest keyPress = {
        .name = "glfwKeyPress",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCacheLocation = &iOCT_inputSystem_inst.keyPressCache
    };
    eOCT_fieldRequest keyRelease = {
        .name = "glfwKeyRelease",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCacheLocation = &iOCT_inputSystem_inst.keyReleaseCache
    };
    eOCT_fieldRequest mouseButton = {
        .name = "glfwMouseButton",
        .type = eOCT_DATATYPE_INT64,
        .optional = false,
        .ticketCacheLocation = &iOCT_inputSystem_inst.mouseButtonCache
    };
    eOCT_fieldRequest mouseButtonPress = {
        .name = "glfwMouseButtonPress",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCacheLocation = &iOCT_inputSystem_inst.mouseButtonPressCache
    };
    eOCT_fieldRequest mouseButtonRelease = {
        .name = "glfwMouseButtonRelease",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCacheLocation = &iOCT_inputSystem_inst.mouseButtonReleaseCache
    };
    eOCT_fieldRequest mouseMoveX = {
        .name = "glfwMouseXPos",
        .type = eOCT_DATATYPE_FLOAT32,
        .optional = false,
        .ticketCacheLocation = &iOCT_inputSystem_inst.mousePositionXCache
    };
    eOCT_fieldRequest mouseMoveY = {
        .name = "glfwMouseYPos",
        .type = eOCT_DATATYPE_FLOAT32,
        .optional = false,
        .ticketCacheLocation = &iOCT_inputSystem_inst.mousePositionYCache
    };
    eOCT_fieldRequest inputRequests[8] = { key, keyPress, keyRelease, mouseButton, mouseButtonPress, mouseButtonRelease, mouseMoveX, mouseMoveY };

    eOCT_systemDescription inputSystem = {
        .name = "INPUT",
        .providedComponents = eOCT_POOL_EMPTY,
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_generateFieldRequestPool(inputRequests, 8),
        .initFx = system_init_INPUT
    };

    iOCT_inputSystem_inst.system = inputSystem;
    eOCT_registry_registerSystem(&iOCT_inputSystem_inst.system);
}