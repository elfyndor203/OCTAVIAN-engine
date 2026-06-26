#include "entityContext_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <inttypes.h>
#include <stdio.h>

#include "ECS/ECS_int.h"

OCT_handle OCT_entityContext_open() {
	OCT_ID contextID = iOCT_entityContext_open();
	OCT_handle handle = {
		.containerID = OCT_ID_ECS,
		.handleType = 0,
		.objectID = contextID,
		.system = OCT_ID_ECS
	};
	return handle;
}
OCT_ID iOCT_entityContext_open() {
	OCT_index newIndex;
	OCT_ID newID;
	iOCT_entityContext* newContext;

	newContext = (iOCT_entityContext*)eOCT_pool_addEntry(&iOCT_ECS_inst.contextPool, &newIndex);	// add context to context list
	newID = eOCT_IDMap_register(&iOCT_ECS_inst.contextMap, newIndex);
	newContext->contextID = newID;

	// init entity pool
	OCT_index entityCapacity = eOCT_POOLSIZE_DEFAULT;
	newContext->entityIDMap = eOCT_IDMap_init(newID, entityCapacity);
	newContext->entityPool = eOCT_pool_init(newID, entityCapacity, iOCT_ECS_inst.entitySize);
	//eOCT_pool_fill(&newContext->entityPool, eOCT_POOL_FILL_ALL, eOCT_POOL_FILL_BYTES, NULL, eOCT_POOL_FILL_BYTES_ONES, 0);
	//eOCT_pool_dump(&newContext->entityPool);

	// init component pools-pool
	size_t componentTotal = iOCT_ECS_inst.componentList.count;
	OCT_index componentCtr;
	eOCT_pool* componentPoolDest;
	size_t componentSize;
	OCT_index indexCheck;

	newContext->componentPools = eOCT_pool_init(newID, componentTotal, sizeof(eOCT_pool));
	printf("Allocated entityContext %"PRIu64"\n", newID);

	// init component pools
	for (componentCtr = 0; componentCtr < componentTotal; componentCtr++) {
		componentSize = *(size_t*)eOCT_pool_access(&iOCT_ECS_inst.componentList, componentCtr, 0);	// add entry to the pool pool
		componentPoolDest = eOCT_pool_addEntry(&newContext->componentPools, &indexCheck);

		*componentPoolDest = eOCT_pool_init(newID, eOCT_POOLSIZE_DEFAULT, componentSize);	// init actual component pool
		//printf("Allocated component #%zu with size %zu\n", indexCheck, componentSize);
	}

	return newID;
}

eOCT_pool* iOCT_getComponentPool(iOCT_entityContext* context, OCT_index componentIndex) {
	eOCT_pool* poolsArray = (eOCT_pool*)context->componentPools.array;
	return &poolsArray[componentIndex];
}

eOCT_pool* eOCT_getComponentPool(OCT_ID contextID, eOCT_componentDescription component) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextID);
	return iOCT_getComponentPool(context, component.componentIndex_reg);
}
eOCT_pool* eOCT_getFieldSourcePool(OCT_ID contextID, eOCT_fieldRequest field) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextID);
	return iOCT_getComponentPool(context, field.componentIndex_reg);
}