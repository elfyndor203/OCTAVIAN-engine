#include "OCT_Core_eng.h"
#include <stddef.h>
#include <stdio.h>

#include "world/world_int.h"
#include "transform2D/transform2D_int.h"

void system_register_WORLD() {
	eOCT_fieldDescription posX = {
		.name = "pos.x",
		.type = eOCT_FIELDTYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, position) + offsetof(OCT_vec2, x),
		.source = eOCT_FIELDPROVIDER_COMPONENT
	};
	eOCT_fieldDescription posY = {
		.name = "pos.y",
		.type = eOCT_FIELDTYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, position) + offsetof(OCT_vec2, y),
		.source = eOCT_FIELDPROVIDER_COMPONENT
	};

	eOCT_fieldDescription transformFields[] = { posX, posY };
	eOCT_componentDescription transform2D = {
		.name = "transform2D",
		.stride = sizeof(iOCT_transform2D),
		.providedFields = eOCT_generateFieldDescriptionPool(transformFields, 2),
		.cacheLocation = &iOCT_world_inst.transform2DCache,
		.rootAttachmentFx = iOCT_transform2D_generateRoot
	};

	eOCT_dataPoolDescription depth = {
		.name = "depth",
		.stride = sizeof(OCT_index),
		.providedFields = eOCT_POOL_EMPTY,
		.cacheLocation = &iOCT_world_inst.depthCache,
		.global = false
	};

	eOCT_dataPoolDescription worldDataPools[] = { depth };
	eOCT_componentDescription worldComponents[] = { transform2D };
	eOCT_systemDescription world = {
		.name = "WORLD",
		.providedComponents = eOCT_generateComponentDescriptionPool(worldComponents, 1),
		.providedDataPools = eOCT_generateDataPoolDescriptionPool(worldDataPools, 1),
		.requestedFields = eOCT_POOL_EMPTY
	};

	iOCT_world_inst.system = world;
	eOCT_registry_registerSystem(&iOCT_world_inst.system);
}