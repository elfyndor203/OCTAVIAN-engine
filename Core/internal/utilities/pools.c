#include "utilities/pools_eng.h"
#include "utilities/types_eng.h"

#include "OCT_Core_eng.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

eOCT_pool eOCT_POOL_EMPTY = {
	.array = NULL,
	.capacity = 0,
	.count = 0,
	.elementSize = 0,
	.ownerID = OCT_ID_NULL
};

eOCT_pool_fillSetting eOCT_POOL_FILLSETTING_NONE = {
	.fillStyle = eOCT_POOL_FILLSTYLE_NONE,
	.value = 0
};
eOCT_pool_fillSetting eOCT_POOL_FILLSETTING_ZEROS = {
	.fillStyle = eOCT_POOL_FILLSTYLE_BYTES,
	.value.byteFill = 0x00
};

static bool eOCT_pool_expand(eOCT_pool* pool, unsigned int factor);

/// <summary>
/// Allocates memory for a single pool. Allows creation of all pools without rewriting when new component types are added
/// </summary>
eOCT_pool eOCT_pool_init(OCT_ID ownerID, OCT_index capacity, size_t elementSize, eOCT_pool_fillSetting fillSetting) {
	eOCT_pool pool = { 0 };

	pool.ownerID = ownerID;					// set default values
	pool.count = 0;
	pool.capacity = capacity;
	pool.elementSize = elementSize;
	pool.fillSetting = fillSetting;
	pool.array = calloc(capacity, elementSize);
	if (!pool.array) {
		OCT_logError(EXIT_FAILED_TO_ALLOCATE);
	}

	if (fillSetting.fillStyle != eOCT_POOL_FILLSTYLE_NONE) {
		eOCT_pool_fill(&pool, fillSetting);
	}
	//printf(">Init pool of size: %zu\n", capacity * elementSize);
	return pool;
}

void* eOCT_pool_addEntry(eOCT_pool* pool, OCT_index* outIndex) {
	if (pool->count == pool->capacity) {
		eOCT_pool_expand(pool, 2);
	}

	void* slot = eOCT_pool_access(pool, pool->count, 0);

	if (outIndex) {
		*outIndex = pool->count;
	}
	pool->count++;

	return slot;
}

OCT_ID eOCT_pool_deleteEntry(eOCT_pool* pool, OCT_index index, bool compact) {
	void* entry = eOCT_pool_access(pool, index, 0);
	OCT_ID swappedID = OCT_ID_NULL;

	// if shuffling is needed
	if (index < pool->count - 1 && compact) {
		void* finalEntry = eOCT_pool_access(pool, pool->count, 0);
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

void* eOCT_pool_access(eOCT_pool* pool, OCT_index index, size_t offset) {
	if (!pool) {
		return NULL;
	}
	if (index >= pool->capacity) {
		return NULL;
	}
	if (offset > pool->elementSize) {
		return NULL;
	}
	void* entry = (char*)pool->array + (size_t)index * pool->elementSize + offset;
	return entry;
}

bool eOCT_pool_combine(eOCT_pool* destination, eOCT_pool* source, bool freeSource) {
	if (destination->elementSize != source->elementSize) {
		return false;
	}

	size_t elementSize = source->elementSize;
	size_t sourceDataSize = elementSize * source->count;
	while (elementSize * (destination->capacity - destination->count) < sourceDataSize) {
		if (!eOCT_pool_expand(destination, 2)) {
			return false;
		}
	}
	memcpy((char*)destination->array + (elementSize * destination->count), source->array, elementSize * source->count);
	destination->count += source->count;

	if (freeSource) {
		eOCT_pool_free(source);
	}
	return true;
}

bool eOCT_pool_fill(const eOCT_pool* pool, eOCT_pool_fillSetting fillSetting) {
	if (fillSetting.fillStyle == eOCT_POOL_FILLSTYLE_NONE) {
		printf("No fill setting chosen\n");
		return 0;
	}

	OCT_index startIndex = pool->count;

	char* array = pool->array;
	if (fillSetting.fillStyle == eOCT_POOL_FILLSTYLE_TYPE) {
		if (!fillSetting.value.typeFill) {
			printf("typeFill is NULL\n");
			return 0;
		}
		for (OCT_index index = startIndex; index < pool->capacity; index++) {
			char* elementStart = array + (index * pool->elementSize);
			memcpy(elementStart, fillSetting.value.typeFill, pool->elementSize);
		}
	}
	else if (fillSetting.fillStyle == eOCT_POOL_FILLSTYLE_BYTES) {
		char* startLoc = array + (startIndex * pool->elementSize);
		memset(startLoc, fillSetting.value.byteFill, pool->elementSize * (pool->capacity - startIndex));
	}
	else if (fillSetting.fillStyle == eOCT_POOL_FILLSTYLE_VALUE) {
		if (pool->elementSize != sizeof(size_t)) {
			printf("Pool size mismatch. \n");
			return 0;
		}
		for (OCT_index index = startIndex; index < pool->capacity; index++) {
			((size_t*)array)[index] = fillSetting.value.valueFill;
		}
	}
	else {
		printf("Something went wrong: pool fill\n");
		return 0;
	}

	printf("Filled pool\n");
	return 1;
}
void eOCT_pool_clear(eOCT_pool* pool) {
	memset(pool->array, 0, pool->capacity * pool->elementSize);
	pool->count = 0;
}
bool eOCT_pool_isEmpty(eOCT_pool pool) {
	if ((!pool.array) || pool.capacity == 0 || pool.count == 0) {
		return true;
	}
	else {
		return false;
	}
}

// Claude generated
void eOCT_pool_dump(eOCT_pool* pool) {
	if (!pool) {
		printf("eOCT_pool_dump: pool is NULL\n");
		return;
	}

	printf("==== eOCT_pool dump (%p) ====\n", (const void*)pool);
	printf("  ownerID      : %"PRIu64"\n", pool->ownerID);
	printf("  count        : %zu\n", (size_t)pool->count);
	printf("  capacity     : %zu\n", (size_t)pool->capacity);
	printf("  elementSize  : %zu bytes\n", pool->elementSize);
	printf("  array ptr    : %p\n", pool->array);

	if (!pool->array || pool->elementSize == 0) {
		printf("  array contents: (empty or invalid)\n");
		printf("==============================\n");
		return;
	}

	printf("  array contents (%zu of %zu capacity):\n", (size_t)pool->count, (size_t)pool->capacity);
	const unsigned char* base = (const unsigned char*)pool->array;
	for (size_t index = 0; index < (size_t)pool->capacity; index++) {
		const unsigned char* elementStart = base + (index * pool->elementSize);
		printf("    [%4zu]%s : ", index, index < (size_t)pool->count ? "*" : " ");
		for (size_t b = 0; b < pool->elementSize; b++) {
			printf("%02X ", elementStart[b]);
		}
		printf("\n");
	}

	printf("==============================\n");
}

static bool eOCT_pool_expand(eOCT_pool* pool, unsigned int factor) {
	if (factor * pool->capacity == 0) {
		return false;
	}

	void* newArray = realloc(pool->array, pool->elementSize * pool->capacity * factor);
	if (!newArray) {
		OCT_logError(EXIT_FAILED_TO_ALLOCATE);
		return false;
	}
	else {
		pool->array = newArray;
		pool->capacity *= factor;
		//printf("%zu\n", pool->capacity * pool->elementSize);
	}

	if (pool->fillSetting.fillStyle != eOCT_POOL_FILLSTYLE_NONE) {
		eOCT_pool_fill(pool, pool->fillSetting);
	}
	return true;
}



