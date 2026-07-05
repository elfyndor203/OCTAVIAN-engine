#include "entity_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>

#include "ECS/ECS_int.h"
#include "entityContext_int.h"

static OCT_index* iOCT_entity_get(iOCT_entityContext* context, OCT_index entityIndex);

OCT_handle OCT_entity_new(OCT_handle contextHandle) {
	printf("Context ID: %zu\n", contextHandle.objectID);
	iOCT_entityContext* context = eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, contextHandle.objectID);
	printf("Context: %p\n", context);
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

	return iOCT_entity_attachComponent(context, entityIndex, component.componentTypeIndex_reg, false, 0);
}
void* eOCT_entity_attachComponentSorted(OCT_handle entity, eOCT_componentDescription component, OCT_index sortValue) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	return iOCT_entity_attachComponent(context, entityIndex, component.componentTypeIndex_reg, true, sortValue);
}
void* iOCT_entity_attachComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex, bool sort, OCT_index sortValue) {
	// goes to the slot in the entity pool
	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
	OCT_index* entityKeyEntry = entityBase + componentTypeIndex; // moves componentIndex elements to the pool index for that component type, of that entity

	// gets the index within the component's pool
	eOCT_pool* componentPool = iOCT_getComponentPool(context, componentTypeIndex);
	OCT_index destinationIndex;
	void* dataLoc;
	if (sort) {
		dataLoc = eOCT_pool_addEntrySorted(componentPool, sortValue, &destinationIndex);
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

// OCT_handle eOCT_entity_genContextHandle(OCT_handle entity) {
// 	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
// 	OCT_handle contextHandle = {
// 		.objectID = entity.containerID,
// 		.containerID = OCT_ID_NULL,
// 		.valid = &context->valid
// 	};
// 	if (*entity.valid) {
// 		return contextHandle;
// 	}
// 	else {
// 		return OCT_HANDLE_NULL;
// 	}
// }

void* eOCT_entity_getField(OCT_handle entity, eOCT_fieldRequest field) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
	if (!context) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad context ID");
	}
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	void* componentLoc = iOCT_entity_getComponent(context, entityIndex, field.providerTypeIndex_reg);

	if (componentLoc == NULL) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Bad Field Access");
		return NULL;
	}
	return (char*)componentLoc + field.fieldOffset_reg;
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
}
void* iOCT_entity_getComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex) {
	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
	OCT_index componentIndex = *(entityBase + componentTypeIndex);

	if (componentIndex == OCT_INDEX_NULL) {
		return NULL;
	}

	eOCT_pool* componentPool = iOCT_getComponentPool(context, componentTypeIndex);
	void* dataLoc = eOCT_pool_access(componentPool, componentIndex, 0);
	return dataLoc;
}

/// <summary>
/// Returns the address of the first component index in the entity pool; essentially the start of the entity
/// </summary>
/// <param name="context"></param>
/// <param name="entityIndex"></param>
/// <returns></returns>
static OCT_index* iOCT_entity_get(iOCT_entityContext* context, OCT_index entityIndex) {
	OCT_index* array = (OCT_index*)context->entityPool.array;
	return &array[entityIndex * iOCT_ECS_inst.componentDescPtrList.count];
}