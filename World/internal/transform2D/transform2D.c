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
	iOCT_transform2D parentTransform = *(iOCT_transform2D*)eOCT_entity_getComponent(parentEntity, iOCT_world_inst.transform2DCache);

	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_attachComponentSorted(entity, iOCT_world_inst.transform2DCache, parentTransform.depth + 1);
	transform->entityID = entity.objectID;
	transform->parentEntityID = parentEntity.objectID;
	transform->position = OCT_vec2_zero;
	transform->rotation = 0.0f;
	transform->scale = (OCT_vec2){1.0f, 1.0f};
	transform->localMatrix = OCT_mat3_identity;
	transform->globalMatrix = parentTransform.globalMatrix;
	transform->depth = parentTransform.depth + 1;

	return true;
}

OCT_vec2 OCT_transform2D_moveTo(OCT_handle entity, OCT_vec2 destination) {
	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_getComponent(entity, iOCT_world_inst.transform2DCache);
	transform->position = destination;

	return transform->position;
}
OCT_vec2 OCT_transform2D_moveBy(OCT_handle entity, OCT_vec2 deltaXY);


OCT_vec2 iOCT_transform2D_setPosition(iOCT_transform2D* transform, OCT_vec2 xy) {
	transform->position = xy;
	return transform->position;
}
OCT_vec2 OCT_transform2D_read(OCT_handle entity, float* rotationOut, OCT_vec2* scaleOut) {
	iOCT_transform2D* transform = (iOCT_transform2D*)eOCT_entity_getComponent(entity, iOCT_world_inst.transform2DCache);

	if (rotationOut) {
		*rotationOut = transform->rotation;
	}
	if (scaleOut) {
		*scaleOut = transform->scale;
	}
	return transform->position;
}

void iOCT_transform2D_generateRoot(OCT_handle rootEntity) {
	iOCT_transform2D* rootTransform = (iOCT_transform2D*)eOCT_entity_attachComponent(rootEntity, iOCT_world_inst.transform2DCache);
	rootTransform->entityID = rootEntity.objectID;
	rootTransform->parentEntityID = OCT_ID_NULL;
	rootTransform->position = OCT_vec2_zero;
	rootTransform->rotation = 0.0f;
	rootTransform->scale = (OCT_vec2){1.0f, 1.0f};
	rootTransform->localMatrix = OCT_mat3_identity;
	rootTransform->globalMatrix = OCT_mat3_identity;
	rootTransform->depth = 0;

	printf("Attached transform2D to ROOT\n");
}