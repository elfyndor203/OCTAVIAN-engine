#include "utilities/IDMap_eng.h"

#include "OCT_Core_eng.h"
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>
#include <stdio.h>

#include "utilities/pools_eng.h"
#include "allocations_int.h"

// Allocates initial memory for a single entityContext.
eOCT_IDMap eOCT_IDMap_open(OCT_ID ownerID, OCT_index capacity) {
	eOCT_IDMap map = { 0 };

	map.ownerID = ownerID;
	map.count = 1; // null ID
	map.array = calloc(capacity + 1, sizeof(OCT_index)); // account for null ID slot
	map.capacity = capacity;
	if (!map.array) {
		OCT_ERROR_LOG(OCT_EXIT_FAILED_TO_ALLOCATE, "IDMap allocation failed");
	}
	//printf(">Init map of size %zu\n", capacity * sizeof(OCT_index));
	if (ownerID != OCT_ID_MEMORY_MANAGER) {
		map.allocationRefIndex = iOCT_memoryManager_logAlloc(ownerID, iOCT_ALLOCATION_MAP, sizeof(OCT_index));
	} else {
		map.allocationRefIndex = OCT_INDEX_NULL;
	}
	return map;
}

// Registers the next available ID with the provided pool index for any new entity or component.
OCT_ID eOCT_IDMap_register(eOCT_IDMap* map, OCT_index inIndex) {
	if (map->count == (map->capacity + 1)) {
		printf("Realloc map of size %zu to size", map->capacity * sizeof(OCT_index));

		void* newArray = realloc(map->array, sizeof(OCT_index) * (map->capacity * 2 + 1));
		if (!newArray) {
			OCT_ERROR_LOG(OCT_EXIT_FAILED_TO_ALLOCATE, "IDMap failed to expand");
			return OCT_ID_NULL;
		}
		else {
			map->array = newArray;
			map->capacity *= 2;
			printf("%zu\n", map->capacity * sizeof(OCT_index));
		}
	}
	OCT_ID newID;

	newID = map->count;		// Grabs the next available ID
	map->count += 1;

	map->array[newID] = inIndex;		// Registers the index with the ID

	return newID;							// only ID gets returned
}

/// <summary>
/// Deletes an ID mapping. Does not deal with free ID recycling yet
/// </summary>
/// <param name="map"></param>
/// <param name="ID"></param>
/// <returns></returns>
OCT_index eOCT_IDMap_deregister(eOCT_IDMap* map, OCT_ID ID) {
	OCT_index index;
	OCT_index* slot = &map->array[ID];
	index = *slot;
	memset(slot, 0, sizeof(OCT_index));
	
	return index;
}


bool eOCT_IDMap_remap(eOCT_IDMap* map, OCT_ID ID, OCT_index newIndex) {
	if (!map) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "IDMap is NULL");
		return false;
	}
	map->array[ID] = newIndex;
	return ID;
}

OCT_index eOCT_IDMap_getIndex(eOCT_IDMap* map, OCT_ID ID) {
	if (ID == OCT_ID_NULL) {
		return OCT_INDEX_NULL;
	}
	if (ID > map->count) {
		OCT_ERROR_LOG(OCT_EXIT_OUT_OF_BOUNDS, "ID does not exist");
		return OCT_INDEX_NULL;
	}
	OCT_index index = map->array[ID];
	if (index == OCT_INDEX_NULL) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "unregistered ID detected: index is NULL"); // __NOTE__: not working correctly right now
		return OCT_INDEX_NULL;
	}
	return map->array[ID];
}

void eOCT_IDMap_free(eOCT_IDMap* map) {
	free(map->array);
	map->array = NULL;

	iOCT_memoryManager_logFree(map->allocationRefIndex);
}