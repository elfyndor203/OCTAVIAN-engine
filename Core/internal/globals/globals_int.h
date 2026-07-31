#pragma once
#include "types_int.h"

#include "layout/types.h"
#include <inttypes.h>
#include "events/events_int.h"

struct iOCT_globals {
    uint64_t frameCount;
    uint64_t timeCountMs;

    eOCT_pool globalSingles;
    iOCT_eventManager globalEvents;
};

extern iOCT_globals iOCT_globals_inst;

void* eOCT_single_get(eOCT_singleDescription* singleDesc);