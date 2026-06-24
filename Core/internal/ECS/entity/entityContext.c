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

	newContext = (iOCT_entityContext*)eOCT_pool_addEntry(&iOCT_ECS_inst.contextPool, &newIndex);
	newID = eOCT_IDMap_register(&iOCT_ECS_inst.contextMap, newIndex);
	newContext->contextID = newID;

	size_t componentTotal = iOCT_ECS_inst.componentList.count;
	OCT_index componentCtr;
	eOCT_pool* componentPoolDest;
	size_t componentSize;
	OCT_index indexCheck;

	newContext->pools = eOCT_pool_init(newID, componentTotal, sizeof(eOCT_pool)); // pool of component pools
	printf("Allocated entityContext %"PRIu64"\n", newID);

	for (componentCtr = 0; componentCtr < componentTotal; componentCtr++) {		// init each component pool
		componentSize = *(size_t*)eOCT_pool_access(&iOCT_ECS_inst.componentList, componentCtr);
		componentPoolDest = eOCT_pool_addEntry(&newContext->pools, &indexCheck);
		*componentPoolDest = eOCT_pool_init(newID, eOCT_POOLSIZE_DEFAULT, componentSize);
		printf("Allocated component #%zu with size %zu\n", indexCheck, componentSize);
	}

	return newID;
}
