#include "camera2D_int.h"
#include "renderer/types_int.h"
#include "renderer/camera2D.h"

#include <assert.h>

#include "renderer/renderer_int.h"
#include "window/types_int.h"
#include "window/window/window_int.h"
#include "window/windowSystem_int.h"

void OCT_camera2D_attach(OCT_local entity, OCT_vec2 position, float rotation, OCT_vec2 viewFrameSize, OCT_global window) {
    if (OCT_local_isNULL(entity) || OCT_global_isNULL(window)) {
        OCT_ERROR_LOG(OCT_EXIT_NULL_HANDLE, "Entity or window does not exist");
    }

    iOCT_camera2D newCamera = {
        .entityHandle = entity,
        .position = position,
        .rotation = rotation,
        .zoom = 1,
        .viewFrameSize = viewFrameSize,
        .cameraMatrix = OCT_mat3_generate(position, viewFrameSize, rotation)
    };
    eOCT_entity_attachComponentOnce(entity, iOCT_renderer_inst.camera2DKey, &newCamera, NULL);
    // iOCT_camera2D* newCamera = eOCT_entity_attachComponent(entity, iOCT_renderer_inst.camera2DKey);
    // assert(newCamera && "Camera creation failed");
    //
    // newCamera->entityHandle = entity;
    // newCamera->position = position;
    // newCamera->rotation = rotation;
    // newCamera->viewFrameSize = viewFrameSize;
    // newCamera->cameraMatrix = OCT_mat3_generate(position, viewFrameSize, rotation);

    // iOCT_window* targetWindow = (iOCT_window*)eOCT_getByID(&iOCT_windowSystem_inst.windowMPool.IDMap, &iOCT_windowSystem_inst.windowMPool.pool, window.objectID);
    iOCT_window* targetWindow = (iOCT_window*)eOCT_mappedPool_getByID(&iOCT_windowSystem_inst.windowMPool, window.objectID);
    targetWindow->activeCameraSourceEntity = entity;
}
void OCT_camera2D_zoomBy(OCT_local entity, float factor) {
    if (OCT_local_isEqual(entity, OCT_CAMERA_FOCUSED)) {
        iOCT_window* focusedWindow = eOCT_mappedPool_getByID(&iOCT_windowSystem_inst.windowMPool, iOCT_windowSystem_inst.focusedWindowID);
        entity = focusedWindow->activeCameraSourceEntity;
    }
    iOCT_camera2D* camera = eOCT_entity_getComponentOnce(entity, iOCT_renderer_inst.camera2DKey);

    camera->zoom = factor;
    camera->cameraMatrix = OCT_mat3_scale(camera->cameraMatrix, (OCT_vec2){factor, factor});
}