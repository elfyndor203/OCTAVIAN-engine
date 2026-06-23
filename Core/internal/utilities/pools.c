#include "utilities/structures/pools_eng.h"
#include "utilities/types_eng.h"

#include "OCT_Core_eng.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <crtdbg.h>
#include <stdio.h>

/// <summary>
/// Allocates memory for a single pool. Allows creation of all pools without rewriting when new component types are added
/// </summary>
eOCT_pool eOCT_pool_init(OCT_ID ownerID, OCT_counter capacity, size_t elementSize) {
	eOCT_pool pool = { 0 };

	pool.ownerID = ownerID;					// set default values
	pool.count = 0;
	pool.capacity = capacity;
	pool.elementSize = elementSize;
	pool.array = calloc(capacity, elementSize);
	if (!pool.array) {
		OCT_logError(EXIT_FAILED_TO_ALLOCATE);
	}

	printf("Init pool of size: %zu\n", capacity * elementSize);
	return pool;
}

void* eOCT_pool_addEntry(eOCT_pool* pool, OCT_index* outIndex) {
	if (pool->count == pool->capacity) {
		printf("Realloc pool of size %zu", pool->capacity * pool->elementSize);

		void* newArray = realloc(pool->array, pool->elementSize * pool->capacity * 2);
		if (!newArray) {
			OCT_logError(EXIT_FAILED_TO_ALLOCATE);
			return NULL;
		}
		else {
			pool->array = newArray;
			pool->capacity *= 2;
			printf("%zu\n", pool->capacity * pool->elementSize);
		}
	}

	void* slot = eOCT_pool_access(pool, pool->count);

	if (outIndex) {
		*outIndex = pool->count;
	}
	pool->count++;

	return slot;
}

OCT_ID eOCT_pool_deleteEntry(eOCT_pool* pool, OCT_index index, bool compact) {
	void* entry = eOCT_pool_access(pool, index);
	OCT_ID swappedID = OCT_ID_NULL;

	// if shuffling is needed
	if (index < pool->count - 1 && compact) {
		void* finalEntry = eOCT_pool_access(pool, pool->count);
		memcpy(entry, finalEntry, pool->elementSize);
		memset(finalEntry, 0, pool->elementSize);
		swappedID = *(OCT_ID*)entry; // return the ID that got swapped to update the IDmap
	}
	else {
		memset(entry, 0, pool->elementSize);
	}
	pool->count--;
	return swappedID;
}

void eOCT_pool_free(eOCT_pool* pool) {
	free(pool->array);
	pool->array = NULL;
}

void* eOCT_pool_access(eOCT_pool* pool, OCT_index index) {
	void* entry = (char*)pool->array + index * pool->elementSize;
	return entry;
}
