#include "transform2D_int.h"
#include "transform2D/transform2D.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>
#include <stdbool.h>

#include "world/world_int.h"

bool OCT_transform2D_attach(OCT_handle entity, OCT_handle parentEntity) {
	if (entity.containerID != parentEntity.containerID){
		printf("Cannot parent to entity in different context\n");
		return false;
	}
	iOCT_transform2D parentTransform = *(iOCT_transform2D*)eOCT_entity_getComponentOnce(parentEntity, iOCT_world_inst.transform2DKey);

	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_attachComponentSorted(entity, iOCT_world_inst.transform2DKey, parentTransform.depth + 1);
	transform->entityHandle = entity;
	transform->parentEntityHandle = parentEntity;
	transform->position = OCT_vec2_zero;
	transform->rotation = 0.0f;
	transform->scale = (OCT_vec2){1.0f, 1.0f};
	transform->localMatrix = OCT_mat3_identity;
	transform->globalMatrix = parentTransform.globalMatrix;
	transform->depth = parentTransform.depth + 1;

	printf("Attached transform with depth %zu\n", transform->depth);
	return true;
}

OCT_vec2 OCT_transform2D_moveTo(OCT_handle entity, OCT_vec2 destination) {
	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_getComponentOnce(entity, iOCT_world_inst.transform2DKey);
	OCT_vec2 originalPosition = transform->position;

	transform->position = destination;

	return OCT_vec2_sub(transform->position, originalPosition);
}
OCT_vec2 OCT_transform2D_moveBy(OCT_handle entity, OCT_vec2 deltaXY);

float OCT_transform2D_rotateTo(OCT_handle entity, float degrees) {
	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_getComponentOnce(entity, iOCT_world_inst.transform2DKey);
	float originalRotation = OCT_rad2deg(transform->rotation);

	transform->rotation = OCT_deg2rad(degrees);

	return transform->rotation - originalRotation;
}

OCT_vec2 iOCT_transform2D_setPosition(iOCT_transform2D* transform, OCT_vec2 xy) {
	transform->position = xy;
	return transform->position;
}
OCT_vec2 OCT_transform2D_read(OCT_handle entity, float* rotationOut, OCT_vec2* scaleOut) {
	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_getComponentOnce(entity, iOCT_world_inst.transform2DKey);

	if (rotationOut) {
		*rotationOut = transform->rotation;
	}
	if (scaleOut) {
		*scaleOut = transform->scale;
	}
	return transform->position;
}

void iOCT_transform2D_generateRoot(OCT_handle rootEntity) {
	iOCT_transform2D* rootTransform = (iOCT_transform2D*)eOCT_entity_attachComponent(rootEntity, iOCT_world_inst.transform2DKey);
	rootTransform->entityHandle = rootEntity;
	rootTransform->parentEntityHandle = rootEntity;
	rootTransform->position = OCT_vec2_zero;
	rootTransform->rotation = 0.0f;
	rootTransform->scale = (OCT_vec2){1.0f, 1.0f};
	rootTransform->localMatrix = OCT_mat3_identity;
	rootTransform->globalMatrix = OCT_mat3_identity;
	rootTransform->depth = iOCT_TRANSFORM_ROOT_DEPTH;
}

// resolves local and global matrices
void iOCT_transform2D_propagate(OCT_handle context) {
	eOCT_pool* transformPool = eOCT_component_getPool(context, iOCT_world_inst.transform2DKey);
	iOCT_transform2D* transformArray = (iOCT_transform2D*)transformPool->array;

	if (!transformPool || !transformArray) {
		OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "Transform array or pool DNE");
		return;
	}
	if (transformPool->count == 0) {	// no transforms -> guaranteed early return
		return;
	}

	iOCT_transform2D* parent;
	iOCT_transform2D* target;
	for (OCT_index transformCtr = 0; transformCtr < transformPool->count; transformCtr++) {
		target = &transformArray[transformCtr];
		target->localMatrix = OCT_mat3_generate(target->position, target->scale, target->rotation);	// resolve local

		if (target->depth == iOCT_TRANSFORM_ROOT_DEPTH) {
			// no parent to handle
			target->globalMatrix = target->localMatrix;
			continue;
		}
		parent = (iOCT_transform2D*)eOCT_entity_getComponentOnce(target->parentEntityHandle, iOCT_world_inst.transform2DKey);
		target->globalMatrix = OCT_mat3_mul(parent->globalMatrix, target->localMatrix);		// resolve global
	}

	//eOCT_pool_dump(transformPool);
}