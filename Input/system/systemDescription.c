
#include "OCT_Core_eng.h"

#include "inputSystem_int.h"
#include "interact/interactBox2D_int.h"

void system_register_INPUT() {
    eOCT_fieldRequest key = {
        .name = "glfwKeys",
        .type = eOCT_DATATYPE_INT64,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.keyTicket
    };
    eOCT_fieldRequest keyPress = {
        .name = "glfwKeyPress",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.keyPressTicket
    };
    eOCT_fieldRequest keyRelease = {
        .name = "glfwKeyRelease",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.keyReleaseTicket
    };
    eOCT_fieldRequest mouseButton = {
        .name = "glfwMouseButton",
        .type = eOCT_DATATYPE_INT64,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.mouseButtonTicket
    };
    eOCT_fieldRequest mouseButtonPress = {
        .name = "glfwMouseButtonPress",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.mouseButtonPressTicket
    };
    eOCT_fieldRequest mouseButtonRelease = {
        .name = "glfwMouseButtonRelease",
        .type = eOCT_DATATYPE_BOOL,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.mouseButtonReleaseTicket
    };
    eOCT_fieldRequest mouseMoveX = {
        .name = "glfwMouseXPos",
        .type = eOCT_DATATYPE_FLOAT32,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.mousePositionXTicket
    };
    eOCT_fieldRequest mouseMoveY = {
        .name = "glfwMouseYPos",
        .type = eOCT_DATATYPE_FLOAT32,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.mousePositionYTicket
    };
    eOCT_fieldRequest mouseScroll = {
        .name = "glfwMouseScrollDelta",
        .type = eOCT_DATATYPE_FLOAT32,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.mouseScrollTicket
    };
    // eOCT_fieldRequest activeCamera = {
    //     .name = "screenToWorldMatrix",
    //     .type = eOCT_DATATYPE_MAT3,
    //     .optional = false,
    //     .ticketCache = &iOCT_inputSystem_inst.cameraMatrixTicket
    // };
    eOCT_fieldRequest deltaTime = {
        .name = "deltaFrameTime",
        .type = eOCT_DATATYPE_DOUBLE64,
        .optional = false,
        .ticketCache = &iOCT_inputSystem_inst.deltaTimeTicket
    };

    eOCT_componentDescription interactBox = {
        .name = "interactBox",
        .keyCacheLocation = &iOCT_inputSystem_inst.interactBoxKey,
        .providedFields = eOCT_POOL_EMPTY,
        .entityHandleValueOffset = offsetof(iOCT_interactBox2D, entity),
        .sort = true,
        .sortValueOffset = offsetof(iOCT_interactBox2D, layer),
        .stride = sizeof(iOCT_interactBox2D)
    };

    eOCT_systemDescription inputSystem = {
        .name = "INPUT",
        .providedComponents = eOCT_generateComponentDescriptionPool(1, interactBox),
        .providedDataPools = eOCT_POOL_EMPTY,
        .providedEvents = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_generateFieldRequestPool(10, key, keyPress, keyRelease, mouseButton, mouseButtonPress, mouseButtonRelease, mouseMoveX, mouseMoveY, mouseScroll, deltaTime),
        .systemInitFx = system_init_INPUT
    };

    iOCT_inputSystem_inst.systemID = eOCT_registry_registerSystem(inputSystem);
}
