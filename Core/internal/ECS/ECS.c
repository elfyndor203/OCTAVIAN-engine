#include "ECS_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>

#include "registry/registry_int.h"
#include "entity/entityContext_int.h"

iOCT_ECS iOCT_ECS_inst = { 0 };

void iOCT_ECS_init() {
	iOCT_ECS_inst.contextMap = eOCT_IDMap_init(OCT_ID_ECS, eOCT_POOLSIZE_DEFAULT);
	iOCT_ECS_inst.contextPool = eOCT_pool_init(OCT_ID_ECS, eOCT_POOLSIZE_DEFAULT, sizeof(iOCT_entityContext));
	iOCT_ECS_inst.componentList = eOCT_pool_init(OCT_ID_ECS, eOCT_POOLSIZE_DEFAULT, sizeof(size_t));

	printf("\nECS initialized:\n");
	printf("\n");
}

OCT_index iOCT_ECS_addComponentType(eOCT_componentDescription desc) {
	iOCT_ECS_inst.entitySize += sizeof(OCT_index);
	OCT_index index = (OCT_index)iOCT_ECS_inst.componentList.count;
	//printf("Contexts will now allocate component %s with index %zu\n", desc.name, index);

	size_t* destination = (size_t*)eOCT_pool_addEntry(&iOCT_ECS_inst.componentList, NULL);	// addEntry after to start at 0
	*destination = desc.stride;

	//printf("New entity size: %zu\n", iOCT_ECS_inst.entitySize);
	return index;
}

eOCT_pool* eOCT_getComponentPool(OCT_ID contextID, OCT_index componentIndex) {
	OCT_index contextIndex = eOCT_IDMap_getIndex(&iOCT_ECS_inst.contextMap, contextID);
	iOCT_entityContext* context = eOCT_pool_access(&iOCT_ECS_inst.contextPool, contextIndex);
	eOCT_pool* poolsArray = (eOCT_pool*)context->componentPools.array;
	return &poolsArray[componentIndex];
}