#pragma once
#include "types_internal.h"

#include "utilities/utilities_core.h"

struct iOCT_registry {
	cOCT_pool systems;
	cOCT_pool fields;
};

extern iOCT_registry iOCT_registry_instance;

void iOCT_registry_init();