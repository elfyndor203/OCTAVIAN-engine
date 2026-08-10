#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_world {
	OCT_ID systemID;

	eOCT_componentDescription transform2DCache;
	eOCT_componentKey transform2DKey;
};

extern iOCT_world iOCT_world_inst;

void iOCT_world_init();