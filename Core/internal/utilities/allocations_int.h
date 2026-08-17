#pragma once

#include "utilities/utilities_eng.h"

typedef enum iOCT_allocationTypes {
    iOCT_ALLOCATION_POOL,
    iOCT_ALLOCATION_MAP
} iOCT_allocationTypes;

typedef struct iOCT_poolRef {
    iOCT_allocationTypes type;
    OCT_ID ownerSystemID;
    size_t elementSize;
    bool freed;
} iOCT_allocationRef;

typedef struct iOCT_memoryManager {
    eOCT_pool allocationRefs;
} iOCT_memoryManager;

OCT_index iOCT_memoryManager_logAlloc(OCT_ID ownerID, iOCT_allocationTypes type, size_t elementSize);
void iOCT_memoryManager_logFree(OCT_index allocationRefIndex);