#pragma once

#include "entityContext_int.h"

void* iOCT_entity_getComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex);
void* iOCT_entity_attachComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentTypeIndex);