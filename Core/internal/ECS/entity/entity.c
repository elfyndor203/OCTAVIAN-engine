#include "entityContext_int.h"
#include "ECS/types_int.h"

#include "entity_int.h"

#include "OCT_Core_eng.h"

#include "ECS/ECS_int.h"

static OCT_index* iOCT_entity_get(iOCT_entityContext* context, OCT_index entityIndex);

void* eOCT_entity_attachComponent(OCT_handle entity, eOCT_componentDescription component) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	return iOCT_entity_attachComponent(context, entityIndex, component.componentTypeIndex_reg);
}
void* iOCT_entity_attachComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex) {
	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
	OCT_index* componentEntry = entityBase + componentTypeIndex; // moves componentIndex elements to the pool index for that component type, of that entity
	
	eOCT_pool* componentPool = iOCT_getComponentPool(context, componentTypeIndex);
	OCT_index componentEntryIndex;
	void* dataLoc = eOCT_pool_addEntry(componentPool, &componentEntryIndex);
	*componentEntry = componentEntryIndex;
	
	return dataLoc;
}

void* eOCT_entity_getComponent(OCT_handle entity, eOCT_componentDescription component) {
	iOCT_entityContext* context = (iOCT_entityContext*)eOCT_getByID(&iOCT_ECS_inst.contextMap, &iOCT_ECS_inst.contextPool, entity.containerID);
	OCT_index entityIndex = eOCT_IDMap_getIndex(&context->entityIDMap, entity.objectID);

	return iOCT_entity_getComponent(context, entityIndex, component.componentTypeIndex_reg);
}
void* iOCT_entity_getComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex) {
	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
	OCT_index componentIndex = *(entityBase + componentTypeIndex);

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
	return &array[entityIndex * iOCT_ECS_inst.componentTypeCount];
}