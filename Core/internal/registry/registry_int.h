#pragma once
#include "types_int.h"

#include "utilities/utilities_eng.h"

struct iOCT_registry {
	eOCT_pool systems;	// systemDesc pointers
	eOCT_pool components; // componentDesc copies
	eOCT_pool dataPools; // dataPoolDesc copies
	eOCT_pool fields;	  // field copies

	bool success;
};

extern iOCT_registry iOCT_registry_inst;
// void iOCT_registerAllSystems(); // defined in build_manifest.c
//void iOCT_allocateAllComponents();