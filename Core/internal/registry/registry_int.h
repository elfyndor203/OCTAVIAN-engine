#pragma once
#include "types_int.h"

#include "utilities/utilities_eng.h"
#include "registry/registry_eng.h"

struct iOCT_registry {
	eOCT_pool systems;	// systemDesc pointers

	eOCT_pool components; // stable copies
	eOCT_pool dataPools;
	eOCT_pool events;
	eOCT_pool globalSingles;

	eOCT_pool fields;	  // field copies

	bool success;
};

extern iOCT_registry iOCT_registry_inst;

eOCT_eventDescription iOCT_registry_findSourceEventDescription(eOCT_fieldRequest fieldRequest);
// void iOCT_registerAllSystems(); // defined in build_manifest.c
//void iOCT_allocateAllComponents();