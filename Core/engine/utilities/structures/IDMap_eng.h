#pragma once
#include "OCT_Core_eng.h"

/// <summary>
/// Maps hold lists of pool array locations, indexed by ID
/// </summary>
struct eOCT_IDMap {
	OCT_ID ownerID;	// Context that owns this map
	OCT_counter count;
	OCT_counter capacity;
	OCT_index* array;
};

eOCT_IDMap eOCT_IDMap_init(OCT_ID ownerID, OCT_counter capacity);
OCT_ID eOCT_IDMap_register(eOCT_IDMap* map, OCT_index inIndex);
OCT_index eOCT_IDMap_deregister(eOCT_IDMap* map, OCT_ID ID);
OCT_ID eOCT_IDMap_remap(eOCT_IDMap* map, OCT_ID ID, OCT_index newIndex);
OCT_index eOCT_IDMap_getIndex(eOCT_IDMap* map, OCT_ID ID);
void eOCT_IDMap_free(eOCT_IDMap* map);