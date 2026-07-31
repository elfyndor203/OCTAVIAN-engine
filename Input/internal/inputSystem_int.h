#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_inputSystem {
    eOCT_systemDescription system;
    eOCT_fieldRequest keyCache;
    eOCT_fieldRequest keyPressCache;
    eOCT_fieldRequest keyReleaseCache;
    eOCT_fieldRequest mouseButtonCache;
    eOCT_fieldRequest mouseButtonPressCache;
    eOCT_fieldRequest mouseButtonReleaseCache;
    eOCT_fieldRequest mousePositionXCache;
    eOCT_fieldRequest mousePositionYCache;

    OCT_vec2 mousePosition;
};

extern iOCT_inputSystem iOCT_inputSystem_inst;

void system_init_INPUT();