#include "OCT_Core_eng.h"
#include <stddef.h>
#include <stdio.h>

#include "world/world_int.h"
#include "transform2D/transform2D_int.h"

void system_register_WORLD() {
	eOCT_fieldDescription posX = {
		.name = "position.x",
		.type = eOCT_DATATYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, position) + offsetof(OCT_vec2, x),
		.providerType = eOCT_DATAPATTERN_COMPONENT
	};
	eOCT_fieldDescription posY = {
		.name = "position.y",
		.type = eOCT_DATATYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, position) + offsetof(OCT_vec2, y),
		.providerType = eOCT_DATAPATTERN_COMPONENT
	};
	eOCT_fieldDescription rotation = {
		.name = "rotation",
		.type = eOCT_DATATYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, rotation),
		.providerType = eOCT_DATAPATTERN_COMPONENT
	};
	eOCT_fieldDescription scaleX = {
		.name = "scale.x",
		.type = eOCT_DATATYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, rotation),
		.providerType = eOCT_DATAPATTERN_COMPONENT
	};
	eOCT_fieldDescription scaleY = {
		.name = "scale.y",
		.type = eOCT_DATATYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, rotation),
		.providerType = eOCT_DATAPATTERN_COMPONENT
	};
	eOCT_fieldDescription matrix2D = {
		.name = "globalTransform2D",
		.type = eOCT_DATATYPE_FLOAT32,
		.offset = offsetof(iOCT_transform2D, globalMatrix),
		.providerType = eOCT_DATAPATTERN_COMPONENT
	};

	eOCT_fieldDescription transformFields[] = { posX, posY, rotation, scaleX, scaleY, matrix2D };
	eOCT_componentDescription transform2D = {
		.name = "transform2D",
		.stride = sizeof(iOCT_transform2D),
		.providedFields = eOCT_generateFieldDescriptionPool(6, posX, posY, rotation, scaleX, scaleY, matrix2D),
		.keyCacheLocation = &iOCT_world_inst.transform2DKey,
		.rootAttachmentFx = iOCT_transform2D_generateRoot,
		.sortValueOffset = offsetof(iOCT_transform2D, depth),
		.entityHandleValueOffset = offsetof(iOCT_transform2D, entityHandle)
	};

	eOCT_systemDescription world = {
		.name = "WORLD",
		.providedComponents = eOCT_generateComponentDescriptionPool(1, transform2D),
		.providedDataPools = eOCT_POOL_EMPTY,
		.requestedFields = eOCT_POOL_EMPTY,
		.initFx = iOCT_world_init
	};

	iOCT_world_inst.systemID = eOCT_registry_registerSystem(world);
}