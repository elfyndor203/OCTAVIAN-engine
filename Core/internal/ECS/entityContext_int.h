#pragma once
#include "ECS/types_int.h"

#include "OCT_Core_eng.h"

#include "events/events_int.h"

#define iOCT_ROOT_ID 1

/// <summary>
/// Opening an entityContext allows you to create entities that interact with each other. Each context manages its own pools and IDmap.
/// </summary>
struct iOCT_entityContext {
	OCT_ID contextID;

	eOCT_IDMap entityIDMap;
	eOCT_pool entityPool;
	eOCT_pool componentPools;
	eOCT_pool systemDataPools;
	eOCT_pool singles;

	iOCT_eventManager eventManager;
};

//iOCT_entityContext* iOCT_entityContext_get(OCT_ID entityContextID);
//eOCT_pool* iOCT_pool_get(iOCT_entityContext* context, OCT_ECSTypes componentType);
iOCT_entityContext* iOCT_entityContext_get(OCT_ID contextID);

eOCT_pool* iOCT_context_getComponentPool(iOCT_entityContext* context, OCT_index componentIndex);
eOCT_pool* iOCT_getDataPool(iOCT_entityContext* context, OCT_index dataPoolTypeIndex);

//void iOCT_entityContext_close(iOCT_entityContext* context);
//void* iOCT_getByID(iOCT_entityContext* context, OCT_ID ID, OCT_ECSTypes componentType);
//void iOCT_entityContext_update(iOCT_entityContext* context);