#pragma once
#include "OCT_Math.h"
#include "cOCT_EngineStructure.h"
#include "physics/types_internal.h"

struct iOCT_PHYModule {
	OCT_vec2 gravity;
	double deltaT;
};

extern iOCT_PHYModule iOCT_PHYModule_instance;
