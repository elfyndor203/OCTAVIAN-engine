#pragma once
#include "types_eng.h"

#include "layout/types.h"
#include "registry/registry_eng.h"

/**
 *
 * @param entity
 * @param component
 * @return componentDataLocation
 */
// void* eOCT_entity_attachComponentOld(OCT_handle entity, eOCT_componentKey component);
// void* eOCT_entity_attachComponentSortedOld(OCT_handle entity, eOCT_componentKey component, OCT_index sortValue);
void* eOCT_entity_attachComponentOnce(OCT_handle entity, eOCT_componentKey componentKey, void* source, OCT_index* outIndex);
void* eOCT_entity_getComponentOnce(OCT_handle entity, eOCT_componentKey component);
void* eOCT_entity_getFieldOnce(OCT_handle entity, eOCT_fieldTicket field);
void* eOCT_entity_getField(eOCT_contextToken contextToken, OCT_handle entity, eOCT_fieldTicket field);
OCT_index eOCT_entity_getComponentIndex(OCT_handle entity, eOCT_componentKey component);
OCT_handle eOCT_entity_getHandle(OCT_handle context, OCT_ID entityID);
OCT_handle eOCT_entity_getContextHandle(OCT_handle entity);
//OCT_handle eOCT_entity_genContextHandle(OCT_handle entity);