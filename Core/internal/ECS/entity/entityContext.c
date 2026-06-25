#include "entityContext_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <inttypes.h>
#include <stdio.h>

#include "ECS/ECS_int.h"

OCT_ID iOCT_entityContext_open() {
	OCT_index newIndex;
	OCT_ID newID;
	iOCT_entityContext* newContext;

	newContext = (iOCT_entityContext*)eOCT_pool_addEntry(&iOCT_ECS_inst.contextPool, &newIndex);	// add context to context list
	newID = eOCT_IDMap_register(&iOCT_ECS_inst.contextMap, newIndex);
	newContext->contextID = newID;

	newContext->entityIDMap = eOCT_IDMap_init(newID, eOCT_POOLSIZE_DEFAULT);
	newContext->entityPool = eOCT_pool_init(newID, eOCT_POOLSIZE_DEFAULT, iOCT_ECS_inst.entitySize);

	size_t componentTotal = iOCT_ECS_inst.componentList.count;
	OCT_index componentCtr;
	eOCT_pool* componentPoolDest;
	size_t componentSize;
	OCT_index indexCheck;

	newContext->componentPools = eOCT_pool_init(newID, componentTotal, sizeof(eOCT_pool)); // init pool of component pools
	printf("Allocated entityContext %"PRIu64"\n", newID);

	for (componentCtr = 0; componentCtr < componentTotal; componentCtr++) {		// init each component pool
		componentSize = *(size_t*)eOCT_pool_access(&iOCT_ECS_inst.componentList, componentCtr);	// add entry to the pool pool
		componentPoolDest = eOCT_pool_addEntry(&newContext->componentPools, &indexCheck);

		*componentPoolDest = eOCT_pool_init(newID, eOCT_POOLSIZE_DEFAULT, componentSize);	// init actual component pool
		printf("Allocated component #%zu with size %zu\n", indexCheck, componentSize);
	}

	return newID;
}
