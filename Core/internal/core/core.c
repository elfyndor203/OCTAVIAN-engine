
#include "OCT_Core.h"

#include "registry/registry_int.h"
#include "ECS/ECS_int.h"

void OCT_engine_init() {
	iOCT_registry_init();

	iOCT_registerAllSystems();

	iOCT_ECS_init();
}