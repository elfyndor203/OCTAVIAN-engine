#pragma once

#include "entityContext_int.h"

OCT_handle iOCT_entity_new(iOCT_entityContext* context);
void* iOCT_entity_getComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentIndex);
void* iOCT_entity_attachComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex);