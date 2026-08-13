#pragma once

#include "entityContext_int.h"
#include <inttypes.h>

struct iOCT_entityMeta {
    OCT_local entity;

    uint64_t componentsAttached;
    uint64_t componentsEnabled;
};

OCT_local iOCT_entity_new(iOCT_entityContext* context);
void* iOCT_entity_getComponent(iOCT_entityContext* context, OCT_index entityIndex, OCT_index componentIndex);
// void* iOCT_entity_attachComponent(iOCT_entityContext* context, OCT_index entityIndex, eOCT_componentKey component, bool sort, OCT_index sortValue);
void iOCT_entity_attachMeta(OCT_local entity);