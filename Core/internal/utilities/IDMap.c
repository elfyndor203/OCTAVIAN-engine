#include "utilities/structures/IDMap_eng.h"

#include "OCT_Core_eng.h"
#include <stdlib.h>
#include <string.h>
#include <crtdbg.h>
#include <stdio.h>

#include "utilities/structures/pools_eng.h"

// Allocates initial memory for a single entityContext.
eOCT_IDMap eOCT_IDMap_init(OCT_ID ownerID, OCT_counter capacity) {
	eOCT_IDMap map = { 0 };

	map.ownerID = ownerID;
	map.count = 1; // null ID
	map.array = calloc(capacity + 1, sizeof(OCT_index)); // account for null ID slot
	map.capacity = capacity;
	if (!map.array) {
		OCT_logError(EXIT_FAILED_TO_ALLOCATE);
	}
	printf("Init map of size %zu\n", capacity * sizeof(OCT_index));
	return map;
}

// Registers the next available ID with the provided pool index for any new entity or component.
OCT_ID eOCT_IDMap_register(eOCT_IDMap* map, OCT_index inIndex) {
	if (map->count == (map->capacity + 1)) {
		printf("Realloc map of size %zu to size", map->capacity * sizeof(OCT_index));

		void* newArray = realloc(map->array, sizeof(OCT_index) * (map->capacity * 2 + 1));
		if (!newArray) {
			OCT_logError(EXIT_FAILED_TO_ALLOCATE);
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


OCT_ID eOCT_IDMap_remap(eOCT_IDMap* map, OCT_ID ID, OCT_index newIndex) {
	map->array[ID] = newIndex;
	return ID;
}

OCT_index eOCT_IDMap_getIndex(eOCT_IDMap* map, OCT_ID ID) {
	if (ID == OCT_ID_NULL) {
		return OCT_index_NULL;
	}
	return map->array[ID];
}

void eOCT_IDMap_free(eOCT_IDMap* map) {
	free(map->array);
	map->array = NULL;
}