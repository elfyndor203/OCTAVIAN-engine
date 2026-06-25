#pragma once
#include "utilities/types_eng.h"

#include "layout/types.h"
#include "OCT_Math.h"

#include <stdbool.h>

#define eOCT_POOLSIZE_DEFAULT 4

/// <summary>
/// Pools manage the arrays of data. Each entitySet can have at most 1 pool per type. The arrays are contigous and allocated when a new pool is created. 
/// </summary>
struct eOCT_pool {
	OCT_ID ownerID;

	OCT_counter count;
	OCT_counter capacity;

	size_t elementSize;
	void* array;
};

eOCT_pool eOCT_pool_init(OCT_ID ownerID, OCT_counter capacity, size_t elementSize);
/// <summary>
/// Returns a void* pointer to the pool slot to be written into which should be cast to the correct type. Optionally returns the index of the slot to be used for ID registration.
/// </summary>
/// <param name="pool"></param>
/// <param name="outIndex"></param>
/// <returns></returns>
void* eOCT_pool_addEntry(eOCT_pool* pool, OCT_index* outIndex);
void* eOCT_pool_access(eOCT_pool* pool, OCT_index index, size_t offset);
bool eOCT_pool_combine(eOCT_pool* destination, eOCT_pool* source, bool freeSource);

OCT_ID eOCT_pool_deleteEntry(eOCT_pool* pool, OCT_index index, bool compact);
void eOCT_pool_free(eOCT_pool* pool);