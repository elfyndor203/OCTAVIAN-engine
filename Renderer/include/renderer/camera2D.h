#pragma once
#include "OCT_Core.h"

#define OCT_CAMERA_FOCUSED OCT_LOCAL_NULL

void OCT_camera2D_attach(OCT_local entity, OCT_vec2 position, float rotation, OCT_vec2 viewFrameSize);
void OCT_camera2D_displayTo(OCT_local entity, OCT_global window);
void OCT_camera2D_zoomBy(OCT_local entity, float factor);

void OCT_camera2D_makeContextScreenSpace(OCT_global context); // __NOTE__ should this go here?