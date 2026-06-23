#pragma once
#include "OCT_Core_eng.h"

#define iOCT_ENTITYCONTEXT_DEFAULT_MAX 64
#define iOCT_ROOT_ID 1
#define iOCT_NO_ENTITYCONTEXT OCT_GENERIC_NONE

/// <summary>
/// Opening an entityContext allows you to create entities that interact with each other. Each context manages its own pools and IDmap.
/// </summary>
struct iOCT_entityContext {
	OCT_ID contextID;

	eOCT_IDMap IDMap;
	eOCT_pool pools;
};

//iOCT_entityContext* iOCT_entityContext_get(OCT_ID entityContextID);
//eOCT_pool* iOCT_pool_get(iOCT_entityContext* context, OCT_ECSTypes componentType);
//
//OCT_ID iOCT_entityContext_open();
//void iOCT_entityContext_close(iOCT_entityContext* context);
//void* iOCT_getByID(iOCT_entityContext* context, OCT_ID ID, OCT_ECSTypes componentType);
//void iOCT_entityContext_update(iOCT_entityContext* context);