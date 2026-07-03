#pragma once
#include "types_int.h"

#include "utilities/utilities_eng.h"

struct iOCT_resources {
    eOCT_IDMap resourceMap;
    eOCT_pool images;
    eOCT_pool audio;
};

extern iOCT_resources iOCT_resources_inst;

void init_OCT_resources_init();