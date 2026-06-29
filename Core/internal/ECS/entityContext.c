#include "entityContext_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <inttypes.h>
#include <stdio.h>

#include "ECS/ECS_int.h"
#include "entity_int.h"

OCT_handle OCT_entityContext_open(OCT_handle* contextOut) {
	OCT_ID contextID;
	OCT_handle rootHandle = iOCT_entityContext_open(&contextID);
	OCT_handle contextHandle = {
		.containerID = OCT_ID_ECS,
		.handleType = 0,
		.objectID = contextID,
		.system = OCT_ID_ECS
	};

	if (contextOut) {
		*contextOut = contextHandle;
	}
	return rootHandle;
}
OCT_handle iOCT_entityContext_open(OCT_ID* contextOut) {
	OCT_index newIndex;
	OCT_ID newID;
	iOCT_entityContext* newContext;

	newContext = (iOCT_entityContext*)eOCT_pool_addEntry(&iOCT_ECS_inst.contextPool, &newIndex);	// add context to context list
	newID = eOCT_IDMap_register(&iOCT_ECS_inst.contextMap, newIndex);
	newContext->contextID = newID;

	// init entity pool
	OCT_index entityCapacity = eOCT_POOL_SIZE_DEFAULT;
	newContext->entityIDMap = eOCT_IDMap_init(newID, entityCapacity);
	newContext->entityPool = eOCT_pool_init(newID, entityCapacity, iOCT_ECS_inst.entitySize, (eOCT_pool_fillSetting){
		                                        .fillStyle = eOCT_POOL_FILLSTYLE_BYTES,
		                                        .value.byteFill = 0xFF
	                                        }); // mark all components indices as unset

	// init component pools-pool
	const OCT_index componentTotal = iOCT_ECS_inst.componentSizeAndOrderList.count;
	OCT_index componentCtr;
	eOCT_pool* componentPoolDest;
	size_t componentSize;
	OCT_index indexCheck;

	newContext->componentPools = eOCT_pool_init(newID, componentTotal, sizeof(eOCT_pool), eOCT_POOL_FILLSETTING_NONE);

	// init component pools
	for (componentCtr = 0; componentCtr < componentTotal; componentCtr++) {
		componentSize = *(size_t*)eOCT_pool_access(&iOCT_ECS_inst.componentSizeAndOrderList, componentCtr, 0);	// add entry to the pool pool
		componentPoolDest = eOCT_pool_addEntry(&newContext->componentPools, &indexCheck);

		*componentPoolDest = eOCT_pool_init(newID, eOCT_POOL_SIZE_DEFAULT, componentSize, eOCT_POOL_FILLSETTING_NONE);	// init actual component pool
		//printf("Allocated component #%zu with size %zu\n", indexCheck, componentSize);
	}

	printf("Allocated entityContext %"PRIu64"\n", newID);

	// init root entity
	OCT_handle rootEntity = iOCT_entity_new(newContext);
	const OCT_index attachTotal = iOCT_ECS_inst.componentRootInitList.count;
	eOCT_rootAttachmentFx* attachFx;
	for (componentCtr = 0; componentCtr < attachTotal; componentCtr++) {
		attachFx = (eOCT_rootAttachmentFx*)eOCT_pool_access(&iOCT_ECS_inst.componentRootInitList, componentCtr, 0);
		if (attachFx) {
			printf("Trying to attach\n");
			(*attachFx)(rootEntity);
		}
		else {
			printf("No attachment\n");
		}
	}
	eOCT_pool_dump(&newContext->entityPool);

	*contextOut = newID;
	return rootEntity;
}



eOCT_pool* eOCT_getComponentPool(OCT_handle contextHandle, eOCT_componentDescription component) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
	return iOCT_getComponentPool(context, component.componentTypeIndex_reg);
}
eOCT_pool* eOCT_getFieldSourcePool(OCT_handle contextHandle, eOCT_fieldRequest field) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
	return iOCT_getComponentPool(context, field.componentTypeIndex_reg);
}
eOCT_pool* eOCT_getDataPool_context(OCT_handle contextHandle, eOCT_dataPoolDescription dataPoolDescription) {
	if (dataPoolDescription.global) {
		printf("Data pool %s is global\n", dataPoolDescription.name);
		return NULL;
	}
	iOCT_entityContext* context = eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
	return iOCT_getDataPool(context, dataPoolDescription.dataPoolTypeIndex_reg);
}

eOCT_pool* iOCT_getComponentPool(iOCT_entityContext* context, OCT_index componentIndex) {
	eOCT_pool* poolsArray = (eOCT_pool*)context->componentPools.array;
	return &poolsArray[componentIndex];
}
eOCT_pool* iOCT_getDataPool(iOCT_entityContext* context, OCT_index dataPoolTypeIndex) {
	eOCT_pool* pool = (eOCT_pool*)eOCT_pool_access(&context->systemDataPools, dataPoolTypeIndex, 0);
	return pool;
}

void OCT_entityContext_dumpEntityPool(OCT_handle contextHandle) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
	eOCT_pool_dump(&context->entityPool);
}
