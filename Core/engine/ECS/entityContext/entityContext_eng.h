#pragma once
#include "ECS/entityContext/entityContext.h"
#include "ECS/components/transform2D/transform2D_internal.h"

#include "OCT_Math.h"
#include "cOCT_EngineStructure.h"
#include "OCT_Errors.h"
#include "ECS/types_internal.h"

#define iOCT_ENTITYCONTEXT_DEFAULT_MAX 64
#define iOCT_ROOT_ID 1
#define iOCT_NO_ENTITYCONTEXT OCT_GENERIC_NONE

/// <summary>
/// Opening an entityContext allows you to create entities that interact with each other. Each context manages its own pools and IDmap.
/// </summary>
struct iOCT_entityContext{
	OCT_ID contextID;

	cOCT_IDMap IDMap;
	cOCT_pool pools[OCT_ECSTypes_total];

	// component utilities
	int currentMaxDepth;
	OCT_index depthEnds[iOCT_TRANSFORM_MAXDEPTH]; // marks the end index of each depth group
};

iOCT_entityContext* iOCT_entityContext_get(OCT_ID entityContextID);
cOCT_pool* iOCT_pool_get(iOCT_entityContext* context, OCT_ECSTypes componentType);

OCT_ID iOCT_entityContext_open();
void iOCT_entityContext_close(iOCT_entityContext* context);
void* iOCT_getByID(iOCT_entityContext* context, OCT_ID ID, OCT_ECSTypes componentType);
void iOCT_entityContext_update(iOCT_entityContext* context);


