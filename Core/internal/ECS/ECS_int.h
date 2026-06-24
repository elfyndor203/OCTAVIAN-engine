#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_ECS {
	size_t entitySize;

	eOCT_IDMap contextMap;
	eOCT_pool contextPool;

	eOCT_pool componentList; // provides sizes and ordering of all components
};

extern iOCT_ECS iOCT_ECS_inst;

void iOCT_ECS_init();
/// <summary>
/// Creates the list of components for each context to generate: component order and sizes
/// </summary>
/// <param name="desc"></param>
/// <returns></returns>
OCT_index iOCT_ECS_addComponent(eOCT_componentDescription desc);
