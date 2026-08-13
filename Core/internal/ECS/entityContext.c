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

static eOCT_pool iOCT_entityContext_initComponentPools();
static eOCT_pool iOCT_entityContext_initDataPools();
static eOCT_pool iOCT_entityContext_initSingles();
static OCT_local iOCT_entityContext_initRootEntity(iOCT_entityContext* context);
static void iOCT_entityContext_initSystems(OCT_global contextHandle);

OCT_global OCT_entityContext_open(OCT_local* rootOut) {
	OCT_ID newID;
	iOCT_entityContext* newContext = eOCT_mappedPool_addEntry(&iOCT_ECS_inst.contextMPool, NULL, &newID, NULL);

	// init entity pool
	OCT_index entityCapacity = eOCT_POOL_CAPACITY_DEFAULT;
	newContext->entityIDMap = eOCT_IDMap_open(newID, entityCapacity);	//
	newContext->entities = eOCT_pool_open(newID, entityCapacity, iOCT_ECS_inst.entitySize);
	eOCT_pool_fillSetting noComponent = {
		.fillStyle = eOCT_POOL_FILLSTYLE_BYTES,
		.value.valueFill = iOCT_COMPONENT_UNSET
	};
	eOCT_pool_setFill(&newContext->entities, noComponent); 	// mark all component indices as unset

	// init components and root entity
	newContext->components = iOCT_entityContext_initComponentPools();
	newContext->dataPools = iOCT_entityContext_initDataPools();
	newContext->singles = iOCT_entityContext_initSingles();
	newContext->events = iOCT_eventManager_open(newID);
	OCT_local rootEntity = iOCT_entityContext_initRootEntity(newContext);
	if (rootOut) {
		*rootOut = rootEntity;
	}

		// finalize
	OCT_global contextHandle = {
		.systemID = OCT_ID_ECS,
		.objectID = newID,
	};

	iOCT_entityContext_initSystems(contextHandle);
	printf("Allocated entityContext %"PRIu64"\n", newID);
	return contextHandle;
}

static eOCT_pool iOCT_entityContext_initComponentPools() {
	OCT_index totalComponents = iOCT_registry_inst.components.count;
	eOCT_pool containerPool = eOCT_pool_open(OCT_ID_ECS, totalComponents, sizeof(eOCT_pool));

	for (OCT_index componentCtr = 0; componentCtr < totalComponents; componentCtr++) {
		eOCT_componentDescription* component = (eOCT_componentDescription*)eOCT_pool_access(&iOCT_registry_inst.components, componentCtr, 0);

		// eOCT_pool* newPool = eOCT_pool_addEntryOld(&containerPool, NULL);
		eOCT_pool* newPool = eOCT_pool_addEntryNew(&containerPool, NULL, NULL);
		*newPool = eOCT_pool_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT, component->stride);	// init actual component pool
		if (component->sortValueOffset != eOCT_POOL_SORT_NONE) {
			eOCT_pool_setSort(newPool, component->sortValueOffset);
		}
		//printf("Allocated component %s with size %zu at %p\n", component->name, component->stride, newPool);
	}

	return containerPool;
}

static eOCT_pool iOCT_entityContext_initDataPools() {
	OCT_index totalDataPools = iOCT_registry_inst.localDataPools.count;
	eOCT_pool containerPool = eOCT_pool_open(OCT_ID_ECS, totalDataPools, sizeof(eOCT_mappedPool));

	for (OCT_index dataPoolCtr = 0; dataPoolCtr < totalDataPools; dataPoolCtr++) {
		eOCT_dataPoolDescription* dataPool = (eOCT_dataPoolDescription*)eOCT_pool_access(&iOCT_registry_inst.localDataPools, dataPoolCtr, 0);

		eOCT_mappedPool* newMPool = eOCT_pool_addEntryNew(&containerPool, NULL, NULL);
		*newMPool = eOCT_mappedPool_open(OCT_ID_ECS, eOCT_POOL_CAPACITY_DEFAULT, dataPool->stride, dataPool->elementIDValueOffset);
		if (dataPool->sort) {
			eOCT_pool_setSort(&newMPool->pool, dataPool->sortValueOffset);
		}
	}
	return containerPool;
}

static eOCT_pool iOCT_entityContext_initSingles() {
	OCT_index localSinglesCt = iOCT_registry_inst.localSingles.count;

	eOCT_pool singlesPool = eOCT_pool_open(OCT_ID_ECS, localSinglesCt, sizeof(eOCT_dataUnion));
	for (OCT_index singleCtr = 0; singleCtr < localSinglesCt; singleCtr++) {
		eOCT_pool_addEntryNew(&singlesPool, NULL, NULL);
	}

	return singlesPool;
}
static OCT_local iOCT_entityContext_initRootEntity(iOCT_entityContext* context) {
	OCT_local rootEntity = iOCT_entity_new(context);

	iOCT_entity_attachMeta(rootEntity);

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

static void iOCT_entityContext_initSystems(OCT_global contextHandle) {
	eOCT_pool systemPool = iOCT_registry_inst.systems;

	for (OCT_index systemCtr = 0; systemCtr < systemPool.count; systemCtr++) {
		eOCT_systemDescription system = *(eOCT_systemDescription*)eOCT_pool_access(&systemPool, systemCtr, 0);
		if (system.contextInitFx) {
			system.contextInitFx(contextHandle);
		}
	}
}

eOCT_contextToken eOCT_context_getToken(OCT_global contextHandle) {
	iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);

	eOCT_contextToken newToken = {
		.contextPtr = context,
		.entityMap = &context->entityIDMap,
		.entities = &context->entities,
		.components = &context->components,
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
eOCT_pool* eOCT_context_getComponentPool(OCT_local contextHandle, eOCT_componentDescription component) {
	iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
	return iOCT_context_getComponentPool(context, component.componentTypeIndex_reg);
}
eOCT_pool* iOCT_context_getComponentPool(iOCT_entityContext* context, OCT_index componentIndex) {
	eOCT_pool* poolsArray = (eOCT_pool*)context->components.array;
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

void OCT_entityContext_dumpEntityPool(OCT_global contextHandle) {
	// iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, contextHandle.objectID);
	eOCT_pool_dump(&context->entities);
}

void eOCT_entityContext_prepare(OCT_global contextHandle) {
	iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
	iOCT_eventManager_clear(&context->events);
}

iOCT_entityContext* iOCT_entityContext_get(OCT_ID contextID) {
	// iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextID);
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_mappedPool_getByID(&iOCT_ECS_inst.contextMPool, contextID);
	return context;
}