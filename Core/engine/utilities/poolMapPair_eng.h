#pragma once
#include "IDMap_eng.h"
#include "pools_eng.h"

void* eOCT_getByID(eOCT_IDMap* map, eOCT_pool* pool, OCT_ID ID);

void* eOCT_pool_addEntrySorted(eOCT_pool* pool, OCT_index sortValue, OCT_index* outIndex);
