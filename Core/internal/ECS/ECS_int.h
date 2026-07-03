#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_ECS {
	size_t entitySize;

	eOCT_IDMap contextMap;
	eOCT_pool contextPool;

	// context instructions
	eOCT_pool componentDescPtrList;
	eOCT_pool componentRootInitList; // functions to call when creating the root entity
	eOCT_pool dataPoolSizeAndOrderList;

	// global data
	eOCT_IDMap globalDataMap;
	eOCT_pool globalDataPools;
};

extern iOCT_ECS iOCT_ECS_inst;

/// <summary>
/// Creates the list of components for each context to generate: component order and sizes
/// </summary>
/// <param name="desc"></param>
/// <returns></returns>
OCT_index iOCT_ECS_addComponentType(eOCT_componentDescription* desc);
OCT_index iOCT_ECS_addDataPool(eOCT_dataPoolDescription desc, bool global);