#include "utilities/mappedPool_eng.h"

#include "OCT_Core_eng.h"
#include <string.h>

#include "utilities/utilities_int.h"

static OCT_ID iOCT_mappedPool_getElementID(eOCT_mappedPool mPool, OCT_index elementIndex);

void* eOCT_getByID(eOCT_IDMap* map, eOCT_pool* pool, OCT_ID ID) {
	OCT_index index = eOCT_IDMap_getIndex(map, ID);
	return eOCT_pool_access(pool, index, 0);
}

OCT_ID eOCT_mappedPool_addEntry(eOCT_mappedPool* mPool, void* source, void* outDestination, OCT_index* outIndex) {
	eOCT_pool* pool = &mPool->pool;
	eOCT_IDMap* map = &mPool->IDMap;

	OCT_index newIndex;
	OCT_ID newID;

	if (pool->sort) {

	}
}
void eOCT_mappedPool_deleteEntry(eOCT_mappedPool* mPool, OCT_ID deletedID) {
	eOCT_pool* pool = &mPool->pool;
	eOCT_IDMap* map = &mPool->IDMap;
	OCT_index deletedIndex = eOCT_IDMap_getIndex(map, deletedID);

	void* deletedElementBase = eOCT_pool_access(pool, deletedIndex, 0);
	void* finalElementBase = eOCT_pool_access(pool, pool->count - 1, 0);
	OCT_index entriesAfterDeleted = pool->count - deletedIndex - 1;
	if (!deletedElementBase || !finalElementBase) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Invalid index provided for delete");
		return;
	}

	if (entriesAfterDeleted > 0) {
		if (pool->sort) { 		// move whole chunk
			for (OCT_index moveIndex = deletedIndex + 1; moveIndex < pool->count; moveIndex++) {
				OCT_ID elementID = iOCT_mappedPool_getElementID(*mPool, moveIndex);
				OCT_index newIndex = moveIndex - 1;
				eOCT_IDMap_remap(map, elementID, newIndex);
			}
			memmove(deletedElementBase, (char*)deletedElementBase + pool->elementSize, entriesAfterDeleted * pool->elementSize);
		}
		else {				// swap remove
			OCT_ID elementID = iOCT_mappedPool_getElementID(*mPool, deletedIndex);
			eOCT_IDMap_remap(map, elementID, deletedIndex);
			memcpy(deletedElementBase, finalElementBase, pool->elementSize);
		}
	}
	pool->count--;
	eOCT_IDMap_deregister(map, deletedID);

	if (pool->fillSetting.fillStyle == eOCT_POOL_FILLSTYLE_NONE) {
		return;
	}
	if (pool->fillSetting.fillStyle != eOCT_POOL_FILLSTYLE_BYTES) {
		OCT_ERROR_LOG(OCT_EXIT_NOT_YET_IMPLEMENTED, "Pool filling not yet finished");
		return;
	}
	memset(finalElementBase, pool->fillSetting.value.byteFill, pool->elementSize);
}

eOCT_mappedPool eOCT_mappedPool_open(OCT_ID systemID, OCT_index capacity, size_t elementSize, size_t elementIDValueOffset) {
	eOCT_mappedPool mPool = {
		.pool = eOCT_pool_open(systemID, capacity, elementSize),
		.IDMap = eOCT_IDMap_open(systemID, capacity),
		.elementIDValueOffset = elementIDValueOffset
	};
	return mPool;
}

static OCT_ID iOCT_mappedPool_getElementID(eOCT_mappedPool mPool, OCT_index elementIndex) {
	void* elementBase = eOCT_pool_access(&mPool.pool, elementIndex, 0);
	OCT_ID elementID = *(OCT_ID*)((char*)elementBase + mPool.elementIDValueOffset);

	return elementID;
}