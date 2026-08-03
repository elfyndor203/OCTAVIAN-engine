#include "inputs_int.h"

#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

OCT_vec2 OCT_mouse_readPosWindow() {
    return iOCT_inputSystem_inst.mousePosition;
}
OCT_vec2 OCT_mouse_readPosWorld() {
    eOCT_pool sourcePool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.cameraMatrixTicket);
    OCT_mat3* screenToWorldMatrix = (OCT_mat3*)eOCT_field_read(sourcePool, iOCT_inputSystem_inst.cameraMatrixTicket, iOCT_inputSystem_inst.cameraMatrixTicket.providerTypeIndex);    // __NOTE__ convoluted, single access is weird, maybe fix
    // OCT_mat3 invCameraMatrix = OCT_mat3_inverse(cameraMatrix);

    OCT_vec2 posWindow = iOCT_inputSystem_inst.mousePosition;
    OCT_vec3 worldPosVec3 = OCT_mat3_mulVec3(*screenToWorldMatrix, (OCT_vec3){posWindow.x, posWindow.y, 1});

    return (OCT_vec2){worldPosVec3.x, worldPosVec3.y};
}