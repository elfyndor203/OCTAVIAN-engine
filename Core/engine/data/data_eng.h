#pragma once
#include "types_eng.h"

#include "layout/types.h"
#include "registry/registry_eng.h"
#include "utilities/utilities_eng.h"

struct eOCT_fieldTicket {
    const char* name;		// for debugging purposes
    eOCT_dataTypes type;
    size_t offsetFromStruct;
    eOCT_dataPattern providerType;
    OCT_index providerTypeIndex;

    bool global;
    eOCT_pool* globalPool;
};

struct eOCT_componentKey {
    const char* name;

    OCT_index componentTypeIndex;
    OCT_index entityHandleValueOffset;
};

struct eOCT_eventKey {
    const char* name;

    OCT_index eventTypeIndex;
    bool global;
    eOCT_pool* globalEventPool;
    eOCT_pool* globalCallbackPool;
};

struct eOCT_dataPoolKey {
    const char* name;

    OCT_index dataPoolTypeIndex;
    bool global;
    eOCT_mappedPool* globalMappedPool;
};

struct eOCT_singleKey {
    const char* name;

    OCT_index singleTypeIndex;
    bool global;
    eOCT_pool* globalPool;
};

struct eOCT_componentExistenceKey {
    const char* name;

    OCT_index componentTypeIndex;
    OCT_ID sourceSystem;
};

eOCT_pool* eOCT_field_getSourcePool(OCT_global contextHandle, eOCT_fieldTicket fieldTicket);
void* eOCT_field_read(eOCT_pool sourcePool, eOCT_fieldTicket fieldDetails, OCT_index entryIndex);
void* eOCT_field_readOnce(eOCT_fieldTicket fieldTicket, OCT_index entryIndex, OCT_global contextHandle);

eOCT_pool* eOCT_component_getPool(OCT_global contextHandle, eOCT_componentKey componentKey);
eOCT_pool* eOCT_event_getPoolGlobal(eOCT_eventKey eventKey, eOCT_pool** callbackPoolOut);
eOCT_pool* eOCT_event_getPoolLocal(eOCT_eventKey eventKey, OCT_global contextHandle, eOCT_pool** callbackPoolOut);
eOCT_dataUnion* eOCT_single_getGlobal(eOCT_singleKey singleKey);
eOCT_dataUnion* eOCT_single_getLocal(eOCT_singleKey singleKey, OCT_global contextHandle);
eOCT_mappedPool* eOCT_dataPool_getGlobal(eOCT_dataPoolKey dataPoolKey);
eOCT_mappedPool* eOCT_dataPool_getLocal(eOCT_dataPoolKey dataPoolKey, OCT_global contextHandle);