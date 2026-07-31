#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_platformSystem {
    eOCT_systemDescription systemDescription;
    eOCT_singleDescription timeCache;
};

extern iOCT_platformSystem iOCT_platformSystem_inst;