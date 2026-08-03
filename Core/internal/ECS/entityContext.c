#include "entityContext_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>

#include "ECS/ECS_int.h"
#include "entity_int.h"
#include "registry/registry_int.h"
#include "events/events_int.h"

#define iOCT_COMPONENT_UNSET 0xFF

static eOCT_pool iOCT_entityContext_initComponentPools(OCT_ID contextID);
static OCT_handle iOCT_entityContext_initRootEntity(iOCT_entityContext* context);

OCT_handle OCT_entityContext_open(OCT_handle* rootOut) {
	// init context details
	// OCT_index newIndex;
	// OCT_ID newID;
	// iOCT_entityContext* newContext;
	iOCT_entityContext* newContext;	// uses a ptr because the newID is used for most init steps, not just set
	OCT_ID newID = eOCT_mappedPool_addEntry(&iOCT_ECS_inst.contextMPool, NULL, (void*)&newContext, NULL);

	// newContext = (iOCT_entityContext*)eOCT_pool_addEntryOld(&iOCT_ECS_inst.contextPool, &newIndex);	// add context to context list
	// newID = eOCT_IDMap_register(&iOCT_ECS_inst.contextMap, newIndex);
	newContext->contextID = newID;
		// init entity pool
	OCT_index entityCapacity = eOCT_POOL_CAPACITY_DEFAULT;
	newContext->entityIDMap = eOCT_IDMap_open(newID, entityCapacity);
	newContext->entityPool = eOCT_pool_open(newID, entityCapacity, iOCT_ECS_inst.entitySize);

	eOCT_pool_fillSetting noComponent = {
		.fillStyle = eOCT_POOL_FILLSTYLE_BYTES,
		.value.valueFill = iOCT_COMPONENT_UNSET
	};
	eOCT_pool_setFill(&newContext->entityPool, noComponent); 	// mark all component indices as unset

	newContext->componentPools = iOCT_entityContext_initComponentPools(newID);
	newContext->eventManager = iOCT_eventManager_open(newID);
	OCT_handle rootEntity = iOCT_entityContext_initRootEntity(newContext);
	if (rootOut) {
		*rootOut = rootEntity;
	}

		// finalize
	OCT_handle contextHandle = {
		.containerID = OCT_ID_NULL,
		.objectID = newID,
	};
	printf("Allocated entityContext %"PRIu64"\n", newID);
	return contextHandle;
}

static eOCT_pool iOCT_entityContext_initComponentPools(OCT_ID contextID) {
	OCT_index totalComponents = iOCT_registry_inst.components.count;
	eOCT_pool containerPool = eOCT_pool_open(contextID, totalComponents, sizeof(eOCT_pool));

	for (OCT_index componentCtr = 0; componentCtr < totalComponents; componentCtr++) {
		eOCT_componentDescription* component = (eOCT_componentDescription*)eOCT_pool_access(&iOCT_registry_inst.components, componentCtr, 0);

		// eOCT_pool* newPool = eOCT_pool_addEntryOld(&containerPool, NULL);
		eOCT_pool* newPool = eOCT_pool_addEntryNew(&containerPool, NULL, NULL);
		*newPool = eOCT_pool_open(contextID, eOCT_POOL_CAPACITY_DEFAULT, component->stride);	// init actual component pool
		if (component->sortValueOffset != eOCT_POOL_SORT_NONE) {
			eOCT_pool_setSort(newPool, component->sortValueOffset);
		}
		//printf("Allocated component %s with size %zu at %p\n", component->name, component->stride, newPool);
	}

	return containerPool;
}

static OCT_handle iOCT_entityContext_initRootEntity(iOCT_entityContext* context) {
	OCT_handle rootEntity = iOCT_entity_new(context);

	const OCT_index componentsTotal = iOCT_registry_inst.components.count;
	for (OCT_index componentCtr = 0; componentCtr < componentsTotal; componentCtr++) {
		eOCT_componentDescription* component = (eOCT_componentDescription*)eOCT_pool_access(&iOCT_registry_inst.components, componentCtr, 0);
		eOCT_rootAttachmentFx attachFx = component->rootAttachmentFx;
		if (attachFx) {
			printf("Attached component %s to ROOT\n", component->name);
			(*attachFx)(rootEntity);
		}
	}

	return rootEntity;
}

eOCT_contextToken eOCT_context_getToken(OCT_handle contextHandle) {
	iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);

	eOCT_contextToken newToken = {
		.contextPtr = context,
		.entityMap = &context->entityIDMap,
		.entities = &context->entityPool,
		.components = &context->componentPools,
		.valid = true
	};

	return newToken;
}
void eOCT_context_invalidateToken(eOCT_contextToken* token) {
	token->contextPtr = NULL;
	token->valid = false;
	token->entityMap = NULL;
	token->entities = NULL;
	token->components = NULL;
}
eOCT_pool* eOCT_context_getComponentPool(OCT_handle contextHandle, eOCT_componentDescription component) {
	iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
	return iOCT_context_getComponentPool(context, component.componentTypeIndex_reg);
}
eOCT_pool* iOCT_context_getComponentPool(iOCT_entityContext* context, OCT_index componentIndex) {
	eOCT_pool* poolsArray = (eOCT_pool*)context->componentPools.array;
	return &poolsArray[componentIndex];
}
// eOCT_pool* eOCT_getFieldSourcePool(OCT_handle contextHandle, eOCT_fieldRequest field) {
// 	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
// 	return iOCT_getComponentPool(context, field.providerIndex_reg);
// }
// eOCT_pool* eOCT_getDataPool_context(OCT_handle contextHandle, eOCT_dataPoolDescription dataPoolDescription) {
// 	if (dataPoolDescription.global) {
// 		printf("Data pool %s is global\n", dataPoolDescription.name);
// 		return NULL;
// 	}
// 	iOCT_entityContext* context = eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
// 	return iOCT_getDataPool(context, dataPoolDescription.dataPoolTypeIndex_reg);
// }


// eOCT_pool* iOCT_getDataPool(iOCT_entityContext* context, OCT_index dataPoolTypeIndex) {
// 	eOCT_pool* pool = (eOCT_pool*)eOCT_pool_access(&context->systemDataPools, dataPoolTypeIndex, 0);
// 	return pool;
// }

void OCT_entityContext_dumpEntityPool(OCT_handle contextHandle) {
	// iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, contextHandle.objectID);
	eOCT_pool_dump(&context->entityPool);
}

void eOCT_entityContext_prepare(OCT_handle contextHandle) {
	iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
	iOCT_eventManager_clear(&context->eventManager);
}

iOCT_entityContext* iOCT_entityContext_get(OCT_ID contextID) {
	// iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextID);
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, contextID);
	return context;
}