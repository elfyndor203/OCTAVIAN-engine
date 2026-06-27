#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_world {
	eOCT_systemDescription system;

	eOCT_componentDescription transform2DCache;
};

extern iOCT_world iOCT_world_inst;

void iOCT_world_init();