#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

#include "registry/registry_int.h"

struct iOCT_ECS {
	size_t entitySize;

	eOCT_IDMap contextMap;
	eOCT_pool contextPool;
};

extern iOCT_ECS iOCT_ECS_inst;

void iOCT_ECS_init();
void iOCT_ECS_addComponent();
