#include "OCT_Core_eng.h"
#include <stddef.h>

#include "../internal/world/world_int.h"
#include "../internal/transform2D/transform2D_int.h"

void system_register_WORLD() {
	eOCT_fieldDescription posX = { "pos.x", eOCT_FIELDTYPE_FLOAT32, offsetof(iOCT_transform2D, position) + offsetof(OCT_vec2, x)};
	eOCT_fieldDescription posY = { "pos.y", eOCT_FIELDTYPE_FLOAT32, offsetof(iOCT_transform2D, position) + offsetof(OCT_vec2, y) };
	eOCT_fieldDescription transformFields[] = { posX, posY };

	eOCT_componentDescription transform2D = {
		.name = "transform2D",
		.stride = sizeof(iOCT_transform2D),
		.providedFields = eOCT_generateFieldDescriptionPool(transformFields, 2),
		.cacheLocation = &iOCT_world_inst.transform2DCache
	};
	eOCT_componentDescription worldComponents[] = { transform2D };

	eOCT_systemDescription world = {
		.name = "WORLD",
		.providedComponents = eOCT_generateComponentDescriptionPool(worldComponents, 1),
		.requestedFields = eOCT_POOL_EMPTY
	};

	iOCT_world_inst.system = world;
	eOCT_registry_registerSystem(&iOCT_world_inst.system);
}