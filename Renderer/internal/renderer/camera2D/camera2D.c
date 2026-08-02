#include "camera2D_int.h"
#include "renderer/types_int.h"

#include <assert.h>

#include "renderer/renderer_int.h"
#include "window/types_int.h"
#include "window/window/window_int.h"
#include "window/windowSystem_int.h"

void OCT_camera2D_attach(OCT_handle entity, OCT_vec2 position, float rotation, OCT_vec2 viewFrameSize, OCT_handle window) {
    if (OCT_handle_isNULL(entity) || OCT_handle_isNULL(window)) {
        OCT_ERROR_LOG(OCT_EXIT_NULL_HANDLE, "Entity or window does not exist");
    }
    iOCT_camera2D* newCamera = eOCT_entity_attachComponent(entity, iOCT_renderer_inst.camera2DKey);
    assert(newCamera && "Camera creation failed");

    newCamera->position = position;
    newCamera->rotation = rotation;
    newCamera->viewFrameSize = viewFrameSize;
    newCamera->cameraMatrix = OCT_mat3_generate(position, viewFrameSize, rotation);

    iOCT_window* targetWindow = (iOCT_window*)eOCT_getByID(&iOCT_windowSystem_inst.windowMap, &iOCT_windowSystem_inst.windowPool, window.objectID);
    targetWindow->activeCameraSourceEntity = entity;
}