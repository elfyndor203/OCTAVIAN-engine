#pragma once
#include "OCT_Core_core.h"

/// <summary>
/// Maps hold lists of pool array locations, indexed by ID
/// </summary>
struct cOCT_IDMap {
	OCT_ID ownerID;	// Context that owns this map
	OCT_counter count;
	OCT_counter capacity;
	OCT_index* array;
};

cOCT_IDMap cOCT_IDMap_init(OCT_ID ownerID, OCT_counter capacity);
OCT_ID cOCT_IDMap_register(cOCT_IDMap* map, OCT_index inIndex);
OCT_index cOCT_IDMap_deregister(cOCT_IDMap* map, OCT_ID ID);
OCT_ID cOCT_IDMap_remap(cOCT_IDMap* map, OCT_ID ID, OCT_index newIndex);
OCT_index cOCT_IDMap_getIndex(cOCT_IDMap* map, OCT_ID ID);
void cOCT_IDMap_free(cOCT_IDMap* map);