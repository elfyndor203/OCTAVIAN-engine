#pragma once

#include "OCT_Core_eng.h"

struct iOCT_camera2D {
    OCT_local entityHandle;

    OCT_vec2 position;
    float rotation;
    float zoom;
    OCT_vec2 viewFrameSize;

    OCT_mat3 cameraMatrix;
};