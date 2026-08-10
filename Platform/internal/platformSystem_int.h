#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_platformSystem {
    OCT_ID systemID;
    eOCT_singleKey timeTicket;
    eOCT_singleKey deltaTimeTicket;
    double previousFrameTime;
};

extern iOCT_platformSystem iOCT_platformSystem_inst;

void iOCT_platformSystem_init();