#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_ECS {
	size_t entitySize;

	// eOCT_IDMap contextMap;
	// eOCT_pool contextPool;
	eOCT_mappedPool contextMPool;

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
void iOCT_ECS_addComponentType();
OCT_index iOCT_ECS_addDataPool(eOCT_dataPoolDescription desc, bool global);
OCT_handle iOCT_getContextHandle(const iOCT_entityContext* context);