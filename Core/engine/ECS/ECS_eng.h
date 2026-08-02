#pragma once
#include "utilities/utilities_eng.h"
#include "registry/registry_eng.h"

struct eOCT_contextToken {
    eOCT_IDMap* entityMap;
    eOCT_pool* entities;
    eOCT_pool* components;
    bool valid;
};

void eOCT_ECS_update();
/*!
 * Clears a context's events to prepare for the next frame.
 * @param contextHandle
 */
void eOCT_entityContext_prepare(OCT_handle contextHandle);

/*!
 * Use to get a context token to access data owned by other systems.
 * @param contextHandle
 * @return
 */
eOCT_contextToken eOCT_context_getToken(OCT_handle contextHandle);

/*!
 * After finishing a pass over a context's data, invalidate the held token so it will not be cached and become stale.
 * @param token
 */
void eOCT_context_invalidateToken(eOCT_contextToken* token);
/*!
 * Use when accessing a self-provided component.
 * @param contextHandle
 * @param component
 * @return componentPoolPtr
 */
// eOCT_pool* eOCT_context_getComponentPool(OCT_handle contextHandle, eOCT_componentDescription component);
eOCT_pool* eOCT_getFieldSourcePool(OCT_handle contextHandle, eOCT_fieldRequest field);
// eOCT_pool* eOCT_getDataPool_context(OCT_handle contextHandle, eOCT_dataPoolDescription dataPoolDescription);
// eOCT_pool* eOCT_getDataPool_global(eOCT_dataPoolDescription dataPoolDescription, eOCT_IDMap** IDMapOut);
// void* eOCT_addGlobalDataEntry(eOCT_dataPoolDescription dataPoolDescription, bool createID, OCT_ID* IDOut);
// void* eOCT_getGlobalDataEntry(eOCT_dataPoolDescription dataPoolDescription, OCT_ID dataID);
// void* eOCT_getGlobalDataField(eOCT_fieldRequest fieldRequest, OCT_ID dataID);
void eOCT_event_broadcast(eOCT_eventKey eventDesc, OCT_handle contextHandle, void* event);
void eOCT_event_subscribe(eOCT_fieldTicket eventField, OCT_handle contextHandle, eOCT_eventCallbackFx callback);
// void* eOCT_event_read(eOCT_fieldRequest eventField, OCT_handle contextHandle, OCT_index eventEntryIndex);
