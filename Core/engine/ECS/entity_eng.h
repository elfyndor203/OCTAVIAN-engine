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
void* eOCT_entity_attachComponent(OCT_handle entity, eOCT_componentDescription component);
void* eOCT_entity_getComponent(OCT_handle entity, eOCT_componentDescription component);
void* eOCT_entity_getField(OCT_handle entity, eOCT_fieldRequest field);
OCT_handle eOCT_entity_getContextHandle(OCT_handle entity);