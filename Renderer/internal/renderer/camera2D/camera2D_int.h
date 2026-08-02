#pragma once

#include "OCT_Core_eng.h"

struct iOCT_camera2D {
    OCT_handle entityHandle;

    OCT_vec2 position;
    float rotation;
    OCT_vec2 viewFrameSize;

    OCT_mat3 cameraMatrix;
};