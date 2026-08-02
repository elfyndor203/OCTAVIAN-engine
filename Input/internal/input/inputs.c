#include "inputs_int.h"

#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

OCT_vec2 OCT_mouse_readPosWindow() {
    return iOCT_inputSystem_inst.mousePosition;
}
OCT_vec2 OCT_mouse_readPosWorld() {
    eOCT_pool cameraSourcePool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.cameraMatrixTicket);
    OCT_mat3 cameraMatrix = *(OCT_mat3*)eOCT_field_read(cameraSourcePool, iOCT_inputSystem_inst.cameraMatrixTicket, iOCT_inputSystem_inst.cameraMatrixTicket.providerTypeIndex);    // __NOTE__ convoluted, single access is weird, maybe fix
    // OCT_mat3 invCameraMatrix = OCT_mat3_inverse(cameraMatrix);

    OCT_vec2 posWindow = iOCT_inputSystem_inst.mousePosition;
    OCT_vec2 windowSize = OCT_mat3_getScale(cameraMatrix);

    OCT_vec2 posNDC = {
        (posWindow.x / windowSize.x) * 2 - 1,
        1 - (posWindow.y / windowSize.y) * 2
    };

    OCT_vec3 windowVec3 = {posNDC.x, posNDC.y, 1};
    OCT_vec3 worldPosVec3 = OCT_mat3_mulVec3(cameraMatrix, windowVec3);

    return (OCT_vec2){worldPosVec3.x, worldPosVec3.y};
}