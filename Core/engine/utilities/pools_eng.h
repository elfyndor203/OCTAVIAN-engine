#pragma once
#include "types_eng.h"

#include "layout/types.h"
#include "OCT_Math.h"

#include <stdbool.h>

#define eOCT_POOL_CAPACITY_DEFAULT 4

#define eOCT_BYTE_ONES 0xFF
#define eOCT_BYTE_ZEROES 0x00

#define eOCT_POOL_SORT_NONE SIZE_MAX

enum eOCT_pool_fillStyles {
	eOCT_POOL_FILLSTYLE_NONE,
	eOCT_POOL_FILLSTYLE_TYPE,
	eOCT_POOL_FILLSTYLE_BYTES,
	eOCT_POOL_FILLSTYLE_VALUE,
};

union eOCT_pool_fillValue {
	const void* typeFill;
	int byteFill;
	OCT_index valueFill;
};

struct eOCT_pool_fillSetting {
	eOCT_pool_fillStyles fillStyle;
	eOCT_pool_fillValue value;
};

/// <summary>
/// A pool manages a single array of data of any type. The array is allocated at pool init and expanded as necessary automatically
/// </summary>
struct eOCT_pool {
	// pool metadata
	OCT_ID ownerID;

	// array metadata
	OCT_index count;
	OCT_index capacity;
	size_t elementSize;
	//void* defaultValue;
	void* array;

	// automation instructions
	eOCT_pool_fillSetting fillSetting;
	size_t sortValueOffset; // offset
	bool sort;
};

extern eOCT_pool eOCT_POOL_EMPTY;
extern eOCT_pool_fillSetting eOCT_POOL_FILLSETTING_NONE;
extern eOCT_pool_fillSetting eOCT_POOL_FILLSETTING_ZEROS;

eOCT_pool eOCT_pool_open(OCT_ID ownerID, OCT_index capacity, size_t elementSize);
/// <summary>
/// Returns a void* pointer to the pool slot to be written into which should be cast to the correct type. Optionally returns the index of the slot to be used for ID registration.
/// </summary>
/// <param name="pool"></param>
/// <param name="outIndex"></param>
/// <returns></returns>
void* eOCT_pool_addEntry(eOCT_pool* pool, OCT_index* outIndex);
void* eOCT_pool_addEntryNew(eOCT_pool* pool, void* source, OCT_index* outIndex);
/**
 * Adds an entry to its sorted position in a pool with a sort setting.
 * @param pool
 * @param sortValue
 * @param outIndex
 * @return
 */
//void* eOCT_pool_addBatch(eOCT_pool* pool, OCT_index count, OCT_index* outIndex_last);
void* eOCT_pool_access(eOCT_pool* pool, OCT_index index, size_t offset);

void eOCT_pool_setFill(eOCT_pool* pool, eOCT_pool_fillSetting fillSetting);
void eOCT_pool_setSort(eOCT_pool* pool, size_t sortValueOffset);

bool eOCT_pool_combine(eOCT_pool* destination, eOCT_pool* source, bool freeSource);

/**
 * Attempts to fill all empty space after the pool contents. Can fill with a type, bytes, or OCT_index value.
 * @param pool
 * @param fillSetting
 * @return success
 */
bool eOCT_pool_fill(const eOCT_pool* pool, eOCT_pool_fillSetting fillSetting);
void eOCT_pool_clear(eOCT_pool* pool);
bool eOCT_pool_isEmpty(eOCT_pool pool);
void eOCT_pool_dump(eOCT_pool* pool);
OCT_index eOCT_pool_expand(eOCT_pool* pool, OCT_index minCapacity);

void eOCT_pool_deleteEntry(eOCT_pool* pool, OCT_index deletedIndex);
void eOCT_pool_free(eOCT_pool* pool);