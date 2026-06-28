#include "ECS_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>

#include "registry/registry_int.h"
#include "entity/entityContext_int.h"

iOCT_ECS iOCT_ECS_inst = { 0 };

void init_OCT_ECS_init() {
	iOCT_ECS_inst.contextMap = eOCT_IDMap_init(OCT_ID_ECS, eOCT_POOL_SIZE_DEFAULT);
	iOCT_ECS_inst.contextPool = eOCT_pool_init(OCT_ID_ECS, eOCT_POOL_SIZE_DEFAULT, sizeof(iOCT_entityContext), eOCT_POOL_FILLSETTING_NONE);
	iOCT_ECS_inst.componentSizeList = eOCT_pool_init(OCT_ID_ECS, eOCT_POOL_SIZE_DEFAULT, sizeof(size_t), eOCT_POOL_FILLSETTING_NONE);
	iOCT_ECS_inst.componentRootInitList = eOCT_pool_init(OCT_ID_ECS, eOCT_POOL_SIZE_DEFAULT, sizeof(eOCT_rootAttachmentFx), eOCT_POOL_FILLSETTING_ZEROS);

	printf("| ECS initialized\n");
}

OCT_index iOCT_ECS_addComponentType(eOCT_componentDescription desc) {
	OCT_index index = iOCT_ECS_inst.componentTypeCount;
	iOCT_ECS_inst.entitySize += sizeof(OCT_index);
	iOCT_ECS_inst.componentTypeCount++;
	//printf("Contexts will now allocate component %s with index %zu\n", desc.name, index);

	size_t* strideDestination = (size_t*)eOCT_pool_addEntry(&iOCT_ECS_inst.componentSizeList, NULL);
	*strideDestination = desc.stride;

	if (desc.rootAttachmentFx) {
		eOCT_rootAttachmentFx* rootAttachDestination = (eOCT_rootAttachmentFx*)eOCT_pool_addEntry(&iOCT_ECS_inst.componentRootInitList, NULL);
		*rootAttachDestination = desc.rootAttachmentFx;
		//printf("Logging root attach function %p\n", desc.rootAttachmentFx);
	}

	//printf("New entity size: %zu\n", iOCT_ECS_inst.entitySize);
	return index;
}