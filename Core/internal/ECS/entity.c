#include "entity_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>

#include "ECS/ECS_int.h"
#include "entityContext_int.h"
#include "registry/registry_int.h"

/*!
 * Gets a pointer to the start of a given entity in the form of the first component slot
 * @param context
 * @param entityIndex
 * @return entityBasePtr
 */
static OCT_index* iOCT_entity_get(iOCT_entityContext* context, OCT_index entityIndex);
/*!
 * Returns the slot in the entity where the given component's index is stored.
 * @param context
 * @param entityIndex
 * @param component
 * @return componentSlotPtr
 */
static OCT_index* iOCT_entity_getComponentSlot(iOCT_entityContext* context, OCT_index entityIndex, eOCT_componentDescription component);
/*!
 * Re-resolves all indices stored in entities. Run after any component pool shuffling.
 * @param context
 * @param componentPool
 * @param component
 */
static void iOCT_entity_resolveIndices(iOCT_entityContext* context, eOCT_pool* componentPool, eOCT_componentDescription component, OCT_index skip);

#pragma region basics
OCT_handle OCT_entity_new(OCT_handle contextHandle) {
	//printf("Context ID: %zu\n", contextHandle.objectID);
	iOCT_entityContext* context = eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
	//printf("Context: %p\n", context);
	return iOCT_entity_new(context);
}
OCT_handle iOCT_entity_new(iOCT_entityContext* context) {
	OCT_index newIndex;
	eOCT_pool_addEntry(&context->entityPool, &newIndex);
	OCT_ID newID = eOCT_IDMap_register(&context->entityIDMap, newIndex);

	return (OCT_handle) {
		.objectID = newID,
		.containerID = context->contextID
	};
}

void* eOCT_entity_attachComponent(OCT_handle entity, eOCT_componentDescription component) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	return iOCT_entity_attachComponent(context, entityIndex, component, false, 0);
}
void* eOCT_entity_attachComponentSorted(OCT_handle entity, eOCT_componentDescription component, OCT_index sortValue) {		// __NOTE__ DOESNT UPDATE OTHER ENTRIES
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	return iOCT_entity_attachComponent(context, entityIndex, component, true, sortValue);
}
void* iOCT_entity_attachComponent(iOCT_entityContext* context, OCT_index entityIndex, eOCT_componentDescription component, bool sort, OCT_index sortValue) {
	// goes to the slot in the entity pool
	OCT_index* entityKeyEntry = iOCT_entity_getComponentSlot(context, entityIndex, component);

	// gets the index within the component's pool
	eOCT_pool* componentPool = iOCT_context_getComponentPool(context, component.componentTypeIndex_reg);
	OCT_index destinationIndex;
	void* dataLoc;
	if (sort) {
		dataLoc = eOCT_pool_addEntrySorted(componentPool, sortValue, &destinationIndex);
		iOCT_entity_resolveIndices(context, componentPool, component, destinationIndex);	// sorting may shuffle other entries
	}
	else {
		dataLoc = eOCT_pool_addEntry(componentPool, &destinationIndex);
	}

	// saves that index in the entity slot
	*entityKeyEntry = destinationIndex;
	//__NOTE__
	//printf("Attached component\n");
	return dataLoc;
}
#pragma endregion

#pragma region accessors
void* eOCT_entity_getField(eOCT_contextToken contextToken, OCT_ID entityID, eOCT_fieldTicket field) {
	if (!contextToken.valid) {
		OCT_ERROR_LOG(OCT_EXIT_STALE_REFERENCE, "Context token invalid");
	}
	OCT_index entityIndex = eOCT_IDMap_getIndex(contextToken.entityMap, entityID);
	if (entityIndex == OCT_INDEX_NULL) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad entity ID");
	}

	OCT_index componentAccessIndex = *(OCT_index*)eOCT_pool_access(contextToken.entities, entityIndex, 0);
	eOCT_pool* componentPool = (eOCT_pool*)eOCT_pool_access(contextToken.components, field.providerTypeIndex, 0);

	void* fieldLoc = eOCT_pool_access(componentPool, componentAccessIndex, field.offsetFromStruct);

	return fieldLoc;
}
void* eOCT_entity_getComponent(OCT_handle entity, eOCT_componentDescription component) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
	if (!context) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad context ID");
	}
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	void* dataLoc = iOCT_entity_getComponent(context, entityIndex, component.componentTypeIndex_reg);
	if (!dataLoc) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad entity ID");
	}
	return dataLoc;
}
void* iOCT_entity_getComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex) {
	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
	OCT_index componentIndex = *(entityBase + componentTypeIndex);

	// printf("Component index: %zu\n", componentIndex);

	if (componentIndex == OCT_INDEX_NULL) {
		return NULL;
	}

	eOCT_pool* componentPool = iOCT_context_getComponentPool(context, componentTypeIndex);
	void* dataLoc = eOCT_pool_access(componentPool, componentIndex, 0);
	return dataLoc;
}
#pragma endregion

#pragma region utils
bool OCT_entity_sameContext(OCT_handle entity1, OCT_handle entity2) {
	if (entity1.containerID == entity2.containerID) {
		return true;
	}
	else {
		return false;
	}
}

bool OCT_entity_fromContext(OCT_handle entity, OCT_handle context) {
	if (entity.containerID == context.objectID) {
		return true;
	}
	else {
		return false;
	}
}

OCT_handle eOCT_entity_getHandle(OCT_handle context, OCT_ID entityID) {
	OCT_handle entityHandle = {
		.containerID = context.objectID,
		.objectID = entityID,
		.handleType = OCT_ID_NULL
	};
	return entityHandle;
}
OCT_handle eOCT_entity_getContextHandle(OCT_handle entity) {
	OCT_handle contextHandle = {
		.containerID = OCT_ID_ECS,
		.objectID = entity.containerID,
		.handleType = OCT_ID_NULL
	};
	return contextHandle;
}
#pragma endregion
// OCT_index eOCT_entity_getComponentIndex(OCT_handle entity, eOCT_componentDescription component) {
// 	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
// 	if (!context) {
// 		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad context ID");
// 	}
// 	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);
//
// 	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
// 	OCT_index componentIndex = *(entityBase + component.componentTypeIndex_reg);
//
// 	return componentIndex;
// }

#pragma region statics
static OCT_index* iOCT_entity_get(iOCT_entityContext* context, OCT_index entityIndex) {
	OCT_index* array = (OCT_index*)context->entityPool.array;
	return &array[entityIndex * iOCT_registry_inst.components.count];
}

static OCT_index* iOCT_entity_getComponentSlot(iOCT_entityContext* context, OCT_index entityIndex, eOCT_componentDescription component) {
	if (!context) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad context ID");
	}
	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
	OCT_index* componentSlot = entityBase + component.componentTypeIndex_reg;

	return componentSlot;
}
static void iOCT_entity_resolveIndices(iOCT_entityContext* context, eOCT_pool* componentPool, eOCT_componentDescription component, OCT_index skip) {
	for (OCT_index compIndex = 0; compIndex < componentPool->count; compIndex++) {
		if (compIndex == skip) {	// for when creating a new entry, the ID will already be resolved by itself later
			continue;
		}
		OCT_ID entityID = *(OCT_ID*)eOCT_pool_access(componentPool, compIndex, component.entitySlotValueOffset);
		OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entityID);
		OCT_index* componentSlot = iOCT_entity_getComponentSlot(context, entityIndex, component);

		*componentSlot = compIndex;
	}
}
#pragma endregion
