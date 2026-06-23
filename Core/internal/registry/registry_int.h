#pragma once
#include "types_int.h"

#include "utilities/utilities_eng.h"

struct iOCT_registry {
	eOCT_pool systems;
	eOCT_pool fields;
	unsigned int componentCount;
};

extern iOCT_registry iOCT_registry_inst;

void iOCT_registry_init();
void iOCT_registerAllSystems(); // defined in build_manifest.c