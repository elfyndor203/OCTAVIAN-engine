#pragma once
#include "types.h"

#include "OCT_Core_eng.h"

bool OCT_transform2D_attach(OCT_handle entity, OCT_handle parentEntity);
OCT_vec2 OCT_transform2D_moveTo(OCT_handle entity, OCT_vec2 destination);
OCT_vec2 OCT_transform2D_read(OCT_handle entity, float* rotationOut, OCT_vec2* scaleOut);