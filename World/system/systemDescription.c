#include "OCT_Core_eng.h"
#include <stddef.h>
#include <stdio.h>

#include "world/world_int.h"
#include "transform2D/transform2D_int.h"

void system_register_WORLD() {
	eOCT_fieldDescription posX = {
		.name = "position.x",
		.type = eOCT_FIELDTYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, position) + offsetof(OCT_vec2, x),
		.provider = eOCT_FIELDPROVIDER_COMPONENT
	};
	eOCT_fieldDescription posY = {
		.name = "position.y",
		.type = eOCT_FIELDTYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, position) + offsetof(OCT_vec2, y),
		.provider = eOCT_FIELDPROVIDER_COMPONENT
	};
	eOCT_fieldDescription rotation = {
		.name = "rotation",
		.type = eOCT_FIELDTYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, rotation),
		.provider = eOCT_FIELDPROVIDER_COMPONENT
	};
	eOCT_fieldDescription scaleX = {
		.name = "scale.x",
		.type = eOCT_FIELDTYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, rotation),
		.provider = eOCT_FIELDPROVIDER_COMPONENT
	};
	eOCT_fieldDescription scaleY = {
		.name = "scale.y",
		.type = eOCT_FIELDTYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, rotation),
		.provider = eOCT_FIELDPROVIDER_COMPONENT
	};
	eOCT_fieldDescription matrix2D = {
		.name = "transform2D",
		.type = eOCT_FIELDTYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, rotation),
		.provider = eOCT_FIELDPROVIDER_COMPONENT
	};

	eOCT_fieldDescription transformFields[] = { posX, posY, rotation, scaleX, scaleY, matrix2D };
	eOCT_componentDescription transform2D = {
		.name = "transform2D",
		.stride = sizeof(iOCT_transform2D),
		.providedFields = eOCT_generateFieldDescriptionPool(transformFields, 6),
		.cacheLocation = &iOCT_world_inst.transform2DCache,
		.rootAttachmentFx = iOCT_transform2D_generateRoot,
		.sortValueOffset = offsetof(iOCT_transform2D, depth),
	};

	eOCT_componentDescription worldComponents[] = { transform2D };
	eOCT_systemDescription world = {
		.name = "WORLD",
		.providedComponents = eOCT_generateComponentDescriptionPool(worldComponents, 1),
		.providedDataPools = eOCT_POOL_EMPTY,
		.requestedFields = eOCT_POOL_EMPTY,
		.initFx = iOCT_world_init
	};

	iOCT_world_inst.system = world;
	eOCT_registry_registerSystem(&iOCT_world_inst.system);
}