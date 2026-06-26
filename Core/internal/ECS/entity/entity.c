#include "entityContext_int.h"
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"

#include "ECS/ECS_int.h"

static OCT_index* iOCT_entity_get(iOCT_entityContext* context, OCT_index entityIndex);

void* eOCT_entity_attachComponent(OCT_ID contextID, OCT_ID entityID, eOCT_componentDescription component) {

}

void* iOCT_entity_attachComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex) {
	OCT_index* entityBase = iOCT_entity_get(context, entityIndex);
	OCT_index* componentEntry = entityBase + componentTypeIndex; // moves componentIndex elements
	
	eOCT_pool* componentPool = iOCT_getComponentPool(context, componentTypeIndex);
	OCT_index componentEntryIndex;
	void* dataLoc = eOCT_pool_addEntry(componentPool, &componentEntryIndex);
	*componentEntry = componentEntryIndex;
	
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