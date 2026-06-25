#include "collider2D_internal.h"
#include "ECS/types_internal.h"

#include "cOCT_EngineStructure.h"
#include <assert.h>

#include "ECS/entityContext/entityContext_internal.h"
#include "ECS/components/physics2D/physics2D_internal.h"

iOCT_collider2D* iOCT_collider2D_get(iOCT_entityContext* context, OCT_ID colliderID) {
    return iOCT_getByID(context, colliderID, OCT_ECSType_collider2D);
}

OCT_handle OCT_hitBox2D_add(OCT_handle entity, OCT_shape2 shape) { // copy of collider2D_add except with hitbox true
	assert(entity.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);

	OCT_ID colliderID = iOCT_collider2D_add(context, entity.objectID, shape, true);

	OCT_handle colliderHandle = {
		.system = OCT_subsystem_ECS,
		.containerID = context->contextID,
		.objectID = colliderID,
		.type = OCT_handle_collider2D
	};

	return colliderHandle;
} 
OCT_handle OCT_collider2D_add(OCT_handle entity, OCT_shape2 shape) {
	assert(entity.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);

	OCT_ID colliderID = iOCT_collider2D_add(context, entity.objectID, shape, false);

	OCT_handle colliderHandle = {
		.system = OCT_subsystem_ECS,
		.containerID = context->contextID,
		.objectID = colliderID,
		.type = OCT_handle_collider2D
	};

	return colliderHandle;
}

OCT_ID iOCT_collider2D_add(iOCT_entityContext* context, OCT_ID entityID, OCT_shape2 shape, bool hitBox) {
	OCT_ID newID;
	OCT_index newIndex;
	iOCT_collider2D* newCollider;
	iOCT_entity* entity;

	entity = iOCT_entity_get(context, entityID);
	newCollider = (iOCT_collider2D*)cOCT_pool_addEntry(iOCT_pool_get(context, OCT_ECSType_collider2D), &newIndex);
	newID = cOCT_IDMap_register(&context->entityIDMap, newIndex);

	newCollider->colliderID = newID;
	newCollider->shape = shape;
	newCollider->hitbox = hitBox;

	entity->colliderID = newID;
	iOCT_entity_updateMask(context, entityID, OCT_ECSType_collider2D);
	
	assert(entity->physicsID != OCT_ID_NULL);
	iOCT_physics2D_get(context, entity->physicsID)->colliderID = newID;
	return newID;
}
