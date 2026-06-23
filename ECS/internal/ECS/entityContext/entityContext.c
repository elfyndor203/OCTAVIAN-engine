#include "entityContext_internal.h"

#include "cOCT_EngineStructure.h"
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>

#include <crtdbg.h>

#include "module/ECSModule_internal.h"
#include "ECS/entity/entity_internal.h"
#include "ECS/components/transform2D/transform2D_internal.h"
#include "ECS/components/sprite2D/sprite2D_internal.h"
#include "ECS/components/physics2D/physics2D_internal.h"
#include "ECS/components/collider2D/collider2D_internal.h"
#include "ECS/components/camera2D/camera2D_internal.h"

static size_t sizeList[OCT_ECSTypes_total] = {
	sizeof(iOCT_entity),
	sizeof(iOCT_transform2D),
	sizeof(iOCT_sprite2D),
	sizeof(iOCT_physics2D),
	sizeof(iOCT_collider2D),
	sizeof(iOCT_camera2D)
};

iOCT_entityContext* iOCT_entityContext_get(OCT_ID contextID) {				// valid as long as the entityContext exists
	OCT_index index = cOCT_IDMap_getIndex(&iOCT_ECSModule_instance.contextMap, contextID);
	return (iOCT_entityContext*)cOCT_pool_access(&iOCT_ECSModule_instance.contextPool, index);
}

cOCT_pool* iOCT_pool_get(iOCT_entityContext* context, OCT_ECSTypes componentType) {
	return &context->pools[componentType];
}

/// <summary>
/// Opens a new entity context in the current ECS instance.
/// </summary>
/// <returns></returns>
OCT_handle OCT_entityContext_open(OCT_handle* outRoot) {
	OCT_handle contextHandle = {
		.system = OCT_subsystem_ECS,
		.containerID = OCT_subsystem_ECS,
		.objectID = iOCT_entityContext_open(),
		.type = OCT_handle_entityContext
	};
	OCT_handle root = {
		.system = OCT_subsystem_ECS,
		.containerID = contextHandle.objectID,
		.objectID = iOCT_ROOT_ID,
		.type = OCT_handle_entity
	};
	*outRoot = root;
	return contextHandle;
}
OCT_ID iOCT_entityContext_open() {
	OCT_index newIndex;
	OCT_ID newID;
	iOCT_entityContext* newContext;

	newContext = (iOCT_entityContext*)cOCT_pool_addEntry(&iOCT_ECSModule_instance.contextPool, &newIndex);
	newID = cOCT_IDMap_register(&iOCT_ECSModule_instance.contextMap, newIndex);

	newContext->contextID = newID;
	newContext->currentMaxDepth = -1; // prepare for root
	memset(&newContext->depthEnds, 0, sizeof(OCT_index) * iOCT_TRANSFORM_MAXDEPTH);

	newContext->IDMap = cOCT_IDMap_init(OCT_subsystem_ECS, cOCT_POOLSIZE_DEFAULT * OCT_ECSTypes_total);	// enough for all pools
	for (int poolType = 0; poolType < OCT_ECSTypes_total; poolType++) {
		newContext->pools[poolType] = cOCT_pool_init(newContext->contextID, cOCT_POOLSIZE_DEFAULT, sizeList[poolType]);
	}

	iOCT_entity_new(newContext, OCT_ID_NULL);						// Create root entity

	return newID;
}

/// <summary>
/// Frees all memory used by the entityContext. Handles bookkeeping by swap replacing with the last entityContext if necessary. 
/// </summary>
/// <param name="closedContextID"></param>
void OCT_entityContext_close(OCT_handle contextHandle) {
	assert(contextHandle.type == OCT_handle_entityContext);

	iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
	iOCT_entityContext_close(context);
}
void iOCT_entityContext_close(iOCT_entityContext* closedContext) {
	OCT_index closedIndex = cOCT_IDMap_deregister(&iOCT_ECSModule_instance.contextMap, closedContext->contextID);

	cOCT_IDMap_free(&closedContext->IDMap);
	cOCT_pool* pool;
	for (int poolType = 0; poolType < OCT_ECSTypes_total; poolType++) {
		pool = &closedContext->pools[poolType];
		cOCT_pool_free(pool);
	}
	
	cOCT_pool_deleteEntry(&iOCT_ECSModule_instance.contextPool, closedIndex, true);
}

/// <summary>
/// Gets a generic pointer to any entity or component in an entityContext. Works as long as the ID is registered, regardless of the state of the entity or component.
/// </summary>
/// <param name="entityContextID"></param>
/// <param name="ID"></param>
/// <param name="type"></param>
/// <returns></returns>
void* iOCT_getByID(iOCT_entityContext* context, OCT_ID ID, OCT_ECSTypes type) {
	if (ID == OCT_ID_NULL) {
		return NULL;
	}

	cOCT_IDMap* map = &context->IDMap;

	OCT_index index = cOCT_IDMap_getIndex(map, ID);
	cOCT_pool* pool = iOCT_pool_get(context, type);

	return cOCT_pool_access(pool, index);
}

void OCT_entityContext_update(OCT_handle contextHandle) {
	assert(contextHandle.type == OCT_handle_entityContext);

	iOCT_entityContext* context = iOCT_entityContext_get(contextHandle.objectID);
	iOCT_entityContext_update(context);
}
void iOCT_entityContext_update(iOCT_entityContext* context) {
	iOCT_transform2D_propagate(context);
}

#pragma region cross-module requests
OCT_counter _OCT_entityContext_getCount() {
	return iOCT_ECSModule_instance.contextPool.count;
}
#pragma endregion

