#pragma once
#include "types_int.h"

#include "utilities/utilities_eng.h"
#include "registry/registry_eng.h"

struct iOCT_eventManager {
    eOCT_pool eventPools;
    eOCT_pool callbackPools;
};

iOCT_eventManager iOCT_eventManager_open(OCT_ID ownerID);
void iOCT_eventManager_clear(iOCT_eventManager* manager);