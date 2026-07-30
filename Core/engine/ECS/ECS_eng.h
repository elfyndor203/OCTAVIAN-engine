#pragma once
#include "utilities/utilities_eng.h"
#include "registry/registry_eng.h"

void eOCT_ECS_update();
void eOCT_entityContext_prepare(OCT_handle contextHandle);
eOCT_pool* eOCT_getComponentPool(OCT_handle contextHandle, eOCT_componentDescription component);
eOCT_pool* eOCT_getFieldSourcePool(OCT_handle contextHandle, eOCT_fieldRequest field);
eOCT_pool* eOCT_getDataPool_context(OCT_handle contextHandle, eOCT_dataPoolDescription dataPoolDescription);
eOCT_pool* eOCT_getDataPool_global(eOCT_dataPoolDescription dataPoolDescription, eOCT_IDMap** IDMapOut);
void* eOCT_addGlobalDataEntry(eOCT_dataPoolDescription dataPoolDescription, bool createID, OCT_ID* IDOut);
void* eOCT_getGlobalDataEntry(eOCT_dataPoolDescription dataPoolDescription, OCT_ID dataID);
void* eOCT_getGlobalDataField(eOCT_fieldRequest fieldRequest, OCT_ID dataID);
void eOCT_event_broadcast(eOCT_eventDescription eventDesc, OCT_handle contextHandle, void* event);
void eOCT_event_subscribe(eOCT_fieldRequest eventField, OCT_handle contextHandle, eOCT_eventCallbackFx callback);
void* eOCT_event_read(eOCT_fieldRequest eventField, OCT_handle contextHandle, OCT_index eventEntryIndex);
