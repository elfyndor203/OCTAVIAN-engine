#include "inputs_int.h"

#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

OCT_vec2 OCT_mouse_readPosWindow() {
    return iOCT_inputSystem_inst.mousePosition;
}
OCT_vec2 OCT_mouse_readPosWorld() {
    eOCT_pool sourcePool = *eOCT_field_getSourcePool(OCT_GLOBAL_NULL, iOCT_inputSystem_inst.cameraMatrixTicket);
    OCT_mat3* screenToWorldMatrix = (OCT_mat3*)eOCT_field_read(sourcePool, iOCT_inputSystem_inst.cameraMatrixTicket, iOCT_inputSystem_inst.cameraMatrixTicket.providerTypeIndex);    // __NOTE__ convoluted, single access is weird, maybe fix
    // OCT_mat3 invCameraMatrix = OCT_mat3_inverse(cameraMatrix);

    OCT_vec2 posWindow = iOCT_inputSystem_inst.mousePosition;
    OCT_vec3 worldPosVec3 = OCT_mat3_mulVec3(*screenToWorldMatrix, (OCT_vec3){posWindow.x, posWindow.y, 1});

    return (OCT_vec2){worldPosVec3.x, worldPosVec3.y};
}
float OCT_mouse_readScroll() {
    return iOCT_inputSystem_inst.mouseScrollDelta;
}

OCT_buttonStates OCT_button_read(OCT_BUTTON button, double* timeSinceLastStateChange_out, OCT_index* framesSinceLastStateChange_out) {
    iOCT_buttonState buttonState = *(iOCT_buttonState*)eOCT_pool_access(&iOCT_inputSystem_inst.buttonStates, button, 0);

    if (timeSinceLastStateChange_out) {
        *timeSinceLastStateChange_out = buttonState.timeSinceLastStateChange;
    }
    if (framesSinceLastStateChange_out) {
        *framesSinceLastStateChange_out = buttonState.framesSinceLastStateChange;
    }
    return buttonState.state;
}

void iOCT_button_updateState(OCT_BUTTON button, OCT_buttonStates pressOrRelease) {
    iOCT_buttonState* buttonState = (iOCT_buttonState*)eOCT_pool_access(&iOCT_inputSystem_inst.buttonStates, button, 0);

    buttonState->state = pressOrRelease;
    buttonState->timeSinceLastStateChange = 0;
    buttonState->framesSinceLastStateChange = 0;
    buttonState->updatedThisFrame = true;
}