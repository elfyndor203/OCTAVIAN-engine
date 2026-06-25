#include "entity_internal.h"

#include "cOCT_EngineStructure.h"
#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <assert.h>

#include "ECS/entityContext/entityContext_internal.h"
#include "ECS/components/transform2D/transform2D_internal.h"

iOCT_entity* iOCT_entity_get(iOCT_entityContext* context, OCT_ID entityID) {
	return (iOCT_entity*)iOCT_getByID(context, entityID, OCT_ECSType_entity);
}

OCT_ID iOCT_entity_getCompID(iOCT_entityContext* context, OCT_ID entityID, OCT_ECSTypes componentType) {
	iOCT_entity* entity = iOCT_entity_get(context, entityID);

	if (!iOCT_entity_hasComponent(context, entityID, componentType)) {
		return OCT_ID_NULL;
	}
	switch (componentType) {
	case OCT_ECSType_transform2D:
		return entity->transformID;
	case OCT_ECSType_sprite2D:
		return entity->spriteID;
	case OCT_ECSType_physics2D:
		return entity->physicsID;
	case OCT_ECSType_collider2D:
		return entity->colliderID;
	case OCT_ECSType_camera2D:
		return entity->cameraID;
	default:
		return OCT_ID_NULL;
	}
}
/// <summary>
/// Spawns a new entity. ParentHandle may be a context or entity handle. If context, the parent of the new entity is the root of that context.
/// </summary>
/// <param name="entityContextID"></param>
/// <param name="parentID"></param>
/// <returns></returns>
OCT_handle OCT_entity_new(OCT_handle parentHandle) {
	assert(parentHandle.type == OCT_handle_entity || parentHandle.type == OCT_handle_entityContext);

	OCT_ID parentID;
	OCT_ID contextID;
	if (parentHandle.type == OCT_handle_entityContext) {	// adding to context
		parentID = iOCT_ROOT_ID;
		contextID = parentHandle.objectID;
	}
	else {	// adding as child
		parentID = parentHandle.objectID;
		contextID = parentHandle.containerID;
	}
	iOCT_entityContext* context = iOCT_entityContext_get(contextID);
	OCT_ID newEntityID = iOCT_entity_new(context, parentID);
	OCT_handle newHandle = {
		.system = OCT_subsystem_ECS,
		.containerID = contextID,
		.objectID = newEntityID,
		.type = OCT_handle_entity
	};
	return newHandle;
}
OCT_ID iOCT_entity_new(iOCT_entityContext* context, OCT_ID parentID) {
	OCT_ID newID;
	iOCT_entity* newEntity;
	OCT_index newIndex;

	newEntity = (iOCT_entity*)cOCT_pool_addEntry(iOCT_pool_get(context, OCT_ECSType_entity), &newIndex);
	newID = cOCT_IDMap_register(&context->entityIDMap, newIndex);		// Register an ID first to enable other functions

	memset(newEntity, 0, sizeof(iOCT_entity)); // fill with NULL IDs

	newEntity->contextID = context->contextID;
	newEntity->entityID = newID;
	newEntity->parentID = parentID;
	newEntity->transformID = iOCT_transform2D_add(context, newID);

	iOCT_entity_updateMask(context, newID, OCT_ECSType_entity);		// dummy marker
	return newID;
}

void OCT_entity_delete(OCT_handle entityHandle) {
	assert(entityHandle.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entityHandle.containerID);
	iOCT_entity_delete(context, entityHandle.objectID);
}
void iOCT_entity_delete(iOCT_entityContext* context, OCT_ID entityID) {
	cOCT_pool* pools = context->componentPools;
	cOCT_IDMap* map = &context->entityIDMap;
	iOCT_entity* entity = iOCT_entity_get(context, entityID);
	OCT_index entityIndex = cOCT_IDMap_getIndex(map, entityID);


	OCT_ID compID;
	OCT_index compIndex;
	OCT_ID swappedCompID;
	for (int comp = 0; comp < OCT_ECSTypes_total; comp++) {
		if (comp == OCT_ECSType_entity || comp == OCT_ECSType_transform2D) { // Entity: handled last. Transform: compaction handled manually to preserve hierarchy
			continue;															
		}
		if (iOCT_entity_hasComponent(context, entityID, comp)) {
			compID = iOCT_entity_getCompID(context, entityID, comp);
			compIndex = cOCT_IDMap_getIndex(&context->entityIDMap, compID);
			swappedCompID = cOCT_pool_deleteEntry(&pools[comp], compIndex, true);
			cOCT_IDMap_remap(map, swappedCompID, compIndex);
			cOCT_IDMap_deregister(map, compID);
			printf("Deleted component %d from entity %" PRIu64 "\n", comp, entityID);
		}
	}
	iOCT_transform2D_delete(context, entityID); // delete the transform
	OCT_ID swappedID = cOCT_pool_deleteEntry(iOCT_pool_get(context, OCT_ECSType_entity), entityIndex, true); // delete the entity
	cOCT_IDMap_remap(map, swappedID, entityIndex);
	cOCT_IDMap_deregister(&context->entityIDMap, entityID); // deregister the ID
}

/// <summary>
/// Checks if a given entity has a given component. 
/// </summary>
/// <param name="entityHandle"></param>
/// <param name="component"></param>
/// <returns></returns>
bool OCT_entity_hasComponent(OCT_handle entityHandle, OCT_ECSTypes component) {
	assert(entityHandle.type == OCT_handle_entity);

	iOCT_entityContext* context = iOCT_entityContext_get(entityHandle.containerID);
	return iOCT_entity_hasComponent(context, entityHandle.objectID, component);
}
bool iOCT_entity_hasComponent(iOCT_entityContext* context, OCT_ID entityID, OCT_ECSTypes componentType) {
	if (iOCT_entity_get(context, entityID)->componentsMask & (1ULL << componentType)) { // creates a new uint_64 with a 1 at the component # bit and compares bitwise
		//printf("entity %zu DOES have componentTypes component #%d\n", entityID, component);
		return true;
	}
//	printf("entity %zu does NOT have componentTypes component #%d\n", entityID, componentType);
	return false;
}

void iOCT_entity_updateMask(iOCT_entityContext* context, OCT_ID entityID, OCT_ECSTypes componentType) {
	iOCT_entity* entity = iOCT_entity_get(context, entityID);
	entity->componentsMask |= (1ULL << componentType);
}

iOCT_entity* iOCT_entity_getParent(iOCT_entityContext* context, OCT_ID entityID) {
	return iOCT_entity_get(context, iOCT_entity_get(context, entityID)->parentID);
}







