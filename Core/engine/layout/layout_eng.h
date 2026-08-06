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

// struct eOCT_fieldTicket_global {
//     const char* name;
//     eOCT_dataTypes type;
//     size_t offsetFromStruct;
//     eOCT_dataPattern providerType;
//     OCT_index providerTypeIndex;
//
//     eOCT_pool* globalPool;
// };

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

struct eOCT_singleKey {
    const char* name;

    OCT_index singleTypeIndex;
    bool global;
    eOCT_pool* globalPool;
};

eOCT_pool* eOCT_field_getSourcePool(OCT_handle contextHandle, eOCT_fieldTicket fieldTicket);
void* eOCT_field_read(eOCT_pool sourcePool, eOCT_fieldTicket fieldDetails, OCT_index entryIndex);
void* eOCT_field_readOnce(eOCT_fieldTicket fieldTicket, OCT_index entryIndex, OCT_handle contextHandle);

eOCT_pool* eOCT_component_getPool(OCT_handle contextHandle, eOCT_componentKey componentKey);
eOCT_pool* eOCT_event_getPool(eOCT_eventKey eventKey, OCT_handle contextHandle, eOCT_pool** callbackPoolOut);
eOCT_dataUnion* eOCT_single_get(eOCT_singleKey singleKey, OCT_handle contextHandle);