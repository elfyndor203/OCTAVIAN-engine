#include "sprite2D_internal.h"
#include "ECS/types_internal.h"

#include "cOCT_EngineStructure.h"
#include "OCT_Math.h"
#include <assert.h>
#include <stdbool.h>

#include "ECS/entityContext/entityContext_internal.h"
#include "_ECS_Output/_ECS_include.h"
#include "module/ECSModule_internal.h"

iOCT_sprite2D* iOCT_sprite2D_get(iOCT_entityContext* context, OCT_ID spriteID) {
	return (iOCT_sprite2D*)iOCT_getByID(context, spriteID, OCT_ECSType_sprite2D);
}

OCT_handle OCT_sprite2D_add(OCT_handle entity, OCT_handle layer, OCT_vec4 color, OCT_vec4 uv, OCT_vec2 dimensions) {
	assert(entity.type == OCT_handle_entity && layer.type == OCT_handle_layer);
	iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);

	OCT_ID spriteID = iOCT_sprite2D_add(context, entity.objectID, layer, color, uv, dimensions);

	OCT_handle spriteHandle = {
		.system = OCT_subsystem_ECS,
		.containerID = context->contextID,
		.objectID = spriteID,
		.type = OCT_handle_sprite2D
	};

	return spriteHandle;
}
OCT_ID iOCT_sprite2D_add(iOCT_entityContext* context, OCT_ID entityID, OCT_handle layer, OCT_vec4 color, OCT_vec4 uv, OCT_vec2 dimensions) {
	OCT_ID newID;
	OCT_index newIndex;
	iOCT_sprite2D* newSprite;
	iOCT_entity* entity = iOCT_entity_get(context, entityID);

	newSprite = (iOCT_sprite2D*)cOCT_pool_addEntry(iOCT_pool_get(context, OCT_ECSType_sprite2D), &newIndex);
	newID = cOCT_IDMap_register(&context->IDMap, newIndex);

	newSprite->spriteID = newID;
	newSprite->uv = uv;
	newSprite->visible = true;
	newSprite->color = color;
	newSprite->dimensions = dimensions;
	newSprite->transform = iOCT_entity_get(context, entityID)->transformID;
	newSprite->layer = layer;

	entity->spriteID = newID;
	entity->componentsMask |= (1ULL << OCT_ECSType_sprite2D);
	return newID;
}

#pragma region cross-module requests
OCT_vec4 _OCT_sprite2D_getUV(OCT_handle spriteHandle) {
	iOCT_entityContext* context = iOCT_entityContext_get(spriteHandle.containerID);
	return iOCT_sprite2D_get(context, spriteHandle.objectID)->uv;
}
OCT_vec4 _OCT_sprite2D_getColor(OCT_handle spriteHandle) {
	iOCT_entityContext* context = iOCT_entityContext_get(spriteHandle.containerID);
	return iOCT_sprite2D_get(context, spriteHandle.objectID)->color;
}
OCT_vec2 _OCT_sprite2D_getDimensions(OCT_handle spriteHandle) {
	iOCT_entityContext* context = iOCT_entityContext_get(spriteHandle.containerID);
	return iOCT_sprite2D_get(context, spriteHandle.objectID)->dimensions;
}

OCT_counter _OCT_sprite2D_getCount(OCT_index contextIndex) {
	iOCT_entityContext* context = &((iOCT_entityContext*)iOCT_ECSModule_instance.contextPool.array)[contextIndex];

	return iOCT_pool_get(context, OCT_ECSType_sprite2D)->count;
}

_OCT_snapshot_sprite _OCT_sprite2D_getSnapshot(OCT_index spriteIndex, OCT_index contextIndex) {
	iOCT_entityContext* context = &((iOCT_entityContext*)iOCT_ECSModule_instance.contextPool.array)[contextIndex];
	iOCT_sprite2D* sprite = &((iOCT_sprite2D*)iOCT_pool_get(context, OCT_ECSType_sprite2D)->array)[spriteIndex];

	_OCT_snapshot_sprite data = {
		.visible = sprite->visible,
		.color = sprite->color,
		.dimensions = sprite->dimensions,
		.uv = sprite->uv,
		.layer = sprite->layer,
		.transform = iOCT_transform2D_get(context, sprite->transform)->globalMatrix
	};
	return data;
}
#pragma endregion