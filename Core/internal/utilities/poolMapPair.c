#include "utilities/structures/poolMapPair_eng.h"

#include "OCT_Core_eng.h"

void* eOCT_getByID(eOCT_IDMap* map, eOCT_pool* pool, OCT_ID ID) {
	OCT_index index = eOCT_IDMap_getIndex(map, ID);
	return eOCT_pool_access(pool, index, 0);
}