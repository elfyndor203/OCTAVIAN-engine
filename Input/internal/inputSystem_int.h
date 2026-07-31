#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_inputSystem {
    eOCT_systemDescription system;
    eOCT_fieldTicket keyCache;
    eOCT_fieldTicket keyPressCache;
    eOCT_fieldTicket keyReleaseCache;
    eOCT_fieldTicket mouseButtonCache;
    eOCT_fieldTicket mouseButtonPressCache;
    eOCT_fieldTicket mouseButtonReleaseCache;
    eOCT_fieldTicket mousePositionXCache;
    eOCT_fieldTicket mousePositionYCache;

    OCT_vec2 mousePosition;
};

extern iOCT_inputSystem iOCT_inputSystem_inst;

void system_init_INPUT();