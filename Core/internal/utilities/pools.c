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

static bool iOCT_pool_expand(eOCT_pool* pool, OCT_index factor);
static void* iOCT_findDestination(eOCT_pool* pool, OCT_index target, OCT_index* outIndex);

#pragma region basic functions
eOCT_pool eOCT_pool_init(OCT_ID ownerID, OCT_index capacity, size_t elementSize) {
	eOCT_pool pool = { 0 };

	pool.ownerID = ownerID;					// set default values
	pool.count = 0;
	pool.capacity = capacity;
	pool.elementSize = elementSize;
	pool.fillSetting = eOCT_POOL_FILLSETTING_NONE;
	pool.sortValueOffset = eOCT_POOL_SORT_NONE;
	pool.array = calloc(capacity, elementSize);
	if (!pool.array) {
		OCT_ERROR_LOG(OCT_EXIT_FAILED_TO_ALLOCATE, "Failed to allocate pool array memory");
	}
	//printf(">Init pool of size: %zu\n", capacity * elementSize);
	return pool;
}
void* eOCT_pool_addEntry(eOCT_pool* pool, OCT_index* outIndex) {
	if (pool->count == pool->capacity) {
		iOCT_pool_expand(pool, 2);
	}
	void* slot = eOCT_pool_access(pool, pool->count, 0);

	if (outIndex) {
		*outIndex = pool->count;
	}
	pool->count++;
	return slot;
}

void* eOCT_pool_addEntrySorted(eOCT_pool* pool, OCT_index sortValue, OCT_index* outIndex) {
	if (pool->sortValueOffset == eOCT_POOL_SORT_NONE) {
		OCT_ERROR_LOG(OCT_ERR_CREATION_FAILED, "Cannot add sorted item to a pool without a sort setting");
		return NULL;
	}
	if (pool->count == pool->capacity) {
		iOCT_pool_expand(pool, 2);
	}

	// shift logic
	OCT_index slotIndex;
	void* slot = iOCT_findDestination(pool, sortValue, &slotIndex);
	void* displaceBase = (char*)slot + pool->elementSize;
	size_t displaceSize = pool->elementSize * (pool->count - slotIndex);
	memmove(displaceBase, slot, displaceSize);

	if (outIndex) {
		*outIndex = slotIndex;
	}
	pool->count++;
	return slot;
}

void* eOCT_pool_access(eOCT_pool* pool, OCT_index index, size_t offset) {
	if (!pool) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Pool DNE");
		return NULL;
	}
	if (index >= pool->capacity) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Index out of range");
		return NULL;
	}
	if (index > pool->count) {
		OCT_ERROR_LOG(OCT_WARNING_UNINITIALIZED, "Pool entry is uninitialized");
	}
	if (offset > pool->elementSize) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Offset greater than element size");
		return NULL;
	}
	void* entry = (char*)pool->array + (size_t)index * pool->elementSize + offset;
	return entry;
}
void eOCT_pool_deleteEntry(eOCT_pool* pool, OCT_index index, bool compact) {
	void* entry = eOCT_pool_access(pool, index, 0);
	// OCT_ID swappedID = OCT_ID_NULL;

	// if shuffling is needed
	if (index < pool->count - 1 && compact) {
		void* finalEntry = eOCT_pool_access(pool, pool->count - 1, 0);
		memcpy(entry, finalEntry, pool->elementSize);
		memset(finalEntry, 0, pool->elementSize);
		// swappedID = *(OCT_ID*)entry; // return the ID that got swapped to update the IDmap
	}
	else {
		memset(entry, 0, pool->elementSize);
	}
	pool->count--;
	// return swappedID;
}
void eOCT_pool_free(eOCT_pool* pool) {
	free(pool->array);
	pool->array = NULL;
}
#pragma endregion

#pragma region batch functions
// void* eOCT_pool_addBatch(eOCT_pool* pool, OCT_index count, OCT_index* outIndex_last) {
// 	while (pool->count + count >= pool->capacity) {
// 		iOCT_pool_expand(pool, 2);
// 	}
//
// 	void* slot = eOCT_pool_access(pool, pool->count, 0);
//
// 	if (outIndex_last) {
// 		*outIndex_last = pool->count + count;
// 	}
// 	pool->count += count;
// 	return slot;
// }
bool eOCT_pool_fill(const eOCT_pool* pool, eOCT_pool_fillSetting fillSetting) {
	if (fillSetting.fillStyle == eOCT_POOL_FILLSTYLE_NONE) {
		printf("No fill setting chosen\n");
		return 0;
	}
	assert(fillSetting.fillStyle != eOCT_POOL_FILLSTYLE_VALUE && "Value fill doesn't work yet\n");

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
	// else if (fillSetting.fillStyle == eOCT_POOL_FILLSTYLE_VALUE) {
	// 	if (pool->elementSize != sizeof(size_t)) {
	// 		printf("Pool size mismatch. \n");
	// 		return 0;
	// 	}
	// 	for (OCT_index index = startIndex; index < pool->capacity; index++) {
	// 		((size_t*)array)[index] = fillSetting.value.valueFill;
	// 	}
	// }
	else {
		printf("Something went wrong: pool fill\n");
		return 0;
	}

	printf("Filled pool\n");
	return 1;
}
void eOCT_pool_clear(eOCT_pool* pool) {
	memset(pool->array, 0, pool->capacity * pool->elementSize);
	// if (pool->count) {
	// 	printf("Cleared %zu entries\n", pool->count);
	// }
	pool->count = 0;
}
bool eOCT_pool_combine(eOCT_pool* destination, eOCT_pool* source, bool freeSource) {
	if (destination->elementSize != source->elementSize) {
		return false;
	}

	size_t elementSize = source->elementSize;
	size_t sourceDataSize = elementSize * source->count;
	while (elementSize * (destination->capacity - destination->count) < sourceDataSize) {
		if (!iOCT_pool_expand(destination, 2)) {
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
#pragma endregion

#pragma region settings
void eOCT_pool_setFill(eOCT_pool* pool, eOCT_pool_fillSetting fillSetting) {
	pool->fillSetting = fillSetting;

	eOCT_pool_fill(pool, pool->fillSetting);
};
void eOCT_pool_setSort(eOCT_pool* pool, size_t sortValueOffset) {
	pool->sortValueOffset = sortValueOffset;

	if (pool->count > 0) {
		OCT_ERROR_LOG(OCT_NOTE_POOL_NOT_EMPTY, "Adding sort setting does not sort existing items");
	}
}
#pragma endregion

#pragma region control functions
OCT_index eOCT_pool_expand(eOCT_pool* pool, OCT_index minCapacity) {
	OCT_index currentCapacity = pool->capacity;
	OCT_index factor = 2;
	while (currentCapacity * factor < minCapacity) {
		factor++;
	}
	iOCT_pool_expand(pool, factor);
	return pool->capacity;
}
static bool iOCT_pool_expand(eOCT_pool* pool, OCT_index factor) {
	if (factor * pool->capacity == 0) {
		return false;
	}

	void* newArray = realloc(pool->array, pool->elementSize * pool->capacity * factor);
	if (!newArray) {
		OCT_ERROR_LOG(OCT_EXIT_FAILED_TO_ALLOCATE, "Failed to expand pool array");
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
#pragma endregion

#pragma region utilities
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
#pragma endregion

static void* iOCT_findDestination(eOCT_pool* pool, OCT_index target, OCT_index* outIndex) {
	char* array = (char*)pool->array;

	OCT_index start = 0;
	OCT_index end = pool->count;
	OCT_index destinationIndex = 0;


	while (start < end) {
		OCT_index mid = start + (end - start) / 2;
		OCT_index midValue = 0;

		midValue = *(OCT_index*)(array + (mid * pool->elementSize) + pool->sortValueOffset);
		if (midValue < target) {
			start = mid + 1;
		}
		else {
			end = mid; // in case mid is equal
		}
	}
	destinationIndex = start;

	if (outIndex) {
		*outIndex = destinationIndex;
	}
	void* destination = array + (destinationIndex * pool->elementSize);
	return destination;
}



