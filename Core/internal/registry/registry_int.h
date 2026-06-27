#pragma once
#include "types_int.h"

#include "utilities/utilities_eng.h"

struct iOCT_registry {
	eOCT_pool systems;
	eOCT_pool components;
	eOCT_pool fields;

	bool success;
};

extern iOCT_registry iOCT_registry_inst;
// void iOCT_registerAllSystems(); // defined in build_manifest.c
//void iOCT_allocateAllComponents();