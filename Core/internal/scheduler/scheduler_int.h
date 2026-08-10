#pragma once
#include "types_int.h"

#include "utilities/utilities_eng.h"
#include "registry/types_eng.h"

struct iOCT_scheduler {
    eOCT_pool updateFxPool;
};

extern iOCT_scheduler iOCT_scheduler_inst;

void iOCT_scheduler_addUpdateFx(eOCT_systemUpdateFx updateFx);