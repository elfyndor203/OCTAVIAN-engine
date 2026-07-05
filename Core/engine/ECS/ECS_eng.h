#pragma once
#include "utilities/utilities_eng.h"
#include "registry/registry_eng.h"

eOCT_pool* eOCT_getComponentPool(OCT_handle contextHandle, eOCT_componentDescription component);
eOCT_pool* eOCT_getFieldSourcePool(OCT_handle contextHandle, eOCT_fieldRequest field);
eOCT_pool* eOCT_getDataPool_context(OCT_handle contextHandle, eOCT_dataPoolDescription dataPoolDescription);
eOCT_pool* eOCT_getDataPool_global(eOCT_dataPoolDescription dataPoolDescription, eOCT_IDMap** IDMapOut);
void* eOCT_addGlobalDataEntry(eOCT_dataPoolDescription dataPoolDescription, bool createID, OCT_ID* IDOut);
void* eOCT_getGlobalDataEntry(eOCT_dataPoolDescription dataPoolDescription, OCT_ID dataID);
void* eOCT_getGlobalDataField(eOCT_fieldRequest fieldRequest, OCT_ID dataID);
