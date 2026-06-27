#pragma once

#include "layout/types.h"
#include "registry/registry_eng.h"

void* eOCT_entity_attachComponent(OCT_handle entity, eOCT_componentDescription component);
void* eOCT_entity_getComponent(OCT_handle entity, eOCT_componentDescription component);