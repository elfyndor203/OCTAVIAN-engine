#include "ECS_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>

#include "registry/registry_int.h"
#include "entity/entityContext_int.h"

iOCT_ECS iOCT_ECS_inst = { 0 };

void iOCT_ECS_init() {
	iOCT_ECS_inst.entitySize = sizeof(OCT_index) * iOCT_registry_inst.componentCount;
	iOCT_ECS_inst.contextMap = eOCT_IDMap_init(OCT_ID_ECS, eOCT_POOLSIZE_DEFAULT);
	iOCT_ECS_inst.contextPool = eOCT_pool_init(OCT_ID_ECS, eOCT_POOLSIZE_DEFAULT, sizeof(iOCT_entityContext));

	printf("\nECS initialized:\n");
	printf("  | Entity size: %zu\n", iOCT_ECS_inst.entitySize);
	printf("  | Contexts capacity: %zu\n", iOCT_ECS_inst.contextPool.capacity);
	printf("\n");
}