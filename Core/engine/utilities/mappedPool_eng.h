#pragma once
#include "IDMap_eng.h"
#include "pools_eng.h"

struct eOCT_mappedPool {
    eOCT_pool pool;
    eOCT_IDMap IDMap;

    size_t elementIDValueOffset;
};

void* eOCT_getByID(eOCT_IDMap* map, eOCT_pool* pool, OCT_ID ID);

void* eOCT_pool_addEntrySorted(eOCT_pool* pool, OCT_index sortValue, OCT_index* outIndex);

eOCT_mappedPool eOCT_mappedPool_open(OCT_ID systemID, OCT_index capacity, size_t elementSize, size_t elementIDValueOffset);

void* eOCT_mappedPool_getByID(eOCT_mappedPool* mPool, OCT_ID ID);
OCT_ID eOCT_mappedPool_addEntry(eOCT_mappedPool* mPool, void* source, void** outDestination, OCT_index* outIndex);
void eOCT_mappedPool_deleteEntry(eOCT_mappedPool* pool, OCT_ID deletedID);