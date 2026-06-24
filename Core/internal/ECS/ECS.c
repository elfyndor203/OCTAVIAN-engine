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

OCT_index iOCT_ECS_addComponent(eOCT_componentDescription desc) {
	size_t* destination = (size_t*)eOCT_pool_addEntry(&iOCT_ECS_inst.componentList, NULL);
	*destination = desc.stride;

	iOCT_ECS_inst.entitySize += sizeof(OCT_index);
	OCT_index index = (OCT_index)iOCT_ECS_inst.componentList.count;
	printf("Contexts will now allocate component %s with index %zu\n", desc.name, index);
	printf("New entity size: %zu\n", iOCT_ECS_inst.entitySize);
	return index;
}