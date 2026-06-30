#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_renderer {
    eOCT_systemDescription systemDescription;
    eOCT_fieldRequest transform2DCache;
};

extern iOCT_renderer iOCT_renderer_inst;