
#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

void system_register_INPUT() {
    eOCT_fieldRequest key = {
        .name = "glfwKeys",
        .type = eOCT_DATATYPE_INT64,
        .optional = false,
        .keyCacheLocation = &iOCT_inputSystem_inst.keyTicket
    };
    eOCT_fieldRequest keyPress = {
        .name = "glfwKeyPress",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .keyCacheLocation = &iOCT_inputSystem_inst.keyPressTicket
    };
    eOCT_fieldRequest keyRelease = {
        .name = "glfwKeyRelease",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .keyCacheLocation = &iOCT_inputSystem_inst.keyReleaseTicket
    };
    eOCT_fieldRequest mouseButton = {
        .name = "glfwMouseButton",
        .type = eOCT_DATATYPE_INT64,
        .optional = false,
        .keyCacheLocation = &iOCT_inputSystem_inst.mouseButtonTicket
    };
    eOCT_fieldRequest mouseButtonPress = {
        .name = "glfwMouseButtonPress",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .keyCacheLocation = &iOCT_inputSystem_inst.mouseButtonPressTicket
    };
    eOCT_fieldRequest mouseButtonRelease = {
        .name = "glfwMouseButtonRelease",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .keyCacheLocation = &iOCT_inputSystem_inst.mouseButtonReleaseTicket
    };
    eOCT_fieldRequest mouseMoveX = {
        .name = "glfwMouseXPos",
        .type = eOCT_DATATYPE_FLOAT32,
        .optional = false,
        .keyCacheLocation = &iOCT_inputSystem_inst.mousePositionXTicket
    };
    eOCT_fieldRequest mouseMoveY = {
        .name = "glfwMouseYPos",
        .type = eOCT_DATATYPE_FLOAT32,
        .optional = false,
        .keyCacheLocation = &iOCT_inputSystem_inst.mousePositionYTicket
    };
    eOCT_fieldRequest activeCamera = {
        .name = "focusedCameraMatrix",
        .type = eOCT_DATATYPE_MAT3,
        .optional = false,
        .keyCacheLocation = &iOCT_inputSystem_inst.cameraMatrixTicket
    };

    eOCT_systemDescription inputSystem = {
        .name = "INPUT",
        .providedComponents = eOCT_POOL_EMPTY,
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_generateFieldRequestPool(9, key, keyPress, keyRelease, mouseButton, mouseButtonPress, mouseButtonRelease, mouseMoveX, mouseMoveY, activeCamera),
        .initFx = system_init_INPUT
    };

    iOCT_inputSystem_inst.system = inputSystem;
    eOCT_registry_registerSystem(&iOCT_inputSystem_inst.system);
}