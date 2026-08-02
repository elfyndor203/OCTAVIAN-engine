#include "globals_int.h"

#include <assert.h>

#include "events/events_int.h"
#include "layout/systems.h"
#include "globals/globals_int.h"
#include "registry/registry_int.h"

iOCT_globals iOCT_globals_inst = { 0 };

void init_OCT_globals_init() {
    iOCT_globals_inst.globalSingles = eOCT_pool_open(OCT_ID_GLOBALS, eOCT_POOL_CAPACITY_DEFAULT, sizeof(eOCT_dataUnion));
}

void init_OCT_globals_build() {
    iOCT_globals_inst.globalEvents = iOCT_eventManager_open(OCT_ID_GLOBALS);
    for (OCT_index singleCtr = 0; singleCtr < iOCT_registry_inst.globalSingles.count; singleCtr++) {
        eOCT_pool_addEntry(&iOCT_globals_inst.globalSingles, NULL);
    }
}

void eOCT_globals_update() {
    iOCT_eventManager_clear(&iOCT_globals_inst.globalEvents);
}