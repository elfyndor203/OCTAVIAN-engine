#include "ECS/components/camera2D/camera2D.h"
#include "camera2D_internal.h"
#include "ECS/types_internal.h"

#include "cOCT_EngineStructure.h"
#include "OCT_Math.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "module/ECSModule_internal.h"
#include "ECS/entityContext/entityContext_internal.h"

iOCT_camera2D* iOCT_camera2D_get(iOCT_entityContext* context, OCT_ID cameraID) {
	return (iOCT_camera2D*)iOCT_getByID(context, cameraID, OCT_ECSType_camera2D);
}

OCT_handle OCT_camera2D_add(OCT_handle entity, OCT_vec2 position, float zoom, float rotation) {
	assert(entity.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);
	OCT_ID cameraID = iOCT_camera2D_add(context, entity.objectID, position, zoom, rotation);

	OCT_handle cameraHandle = {
		.containerID = entity.containerID,
		.objectID = cameraID,
		.system = OCT_subsystem_ECS,
		.type = OCT_handle_camera2D
	};

	return cameraHandle;
}
OCT_ID iOCT_camera2D_add(iOCT_entityContext* context, OCT_ID entityID, OCT_vec2 position, float zoom, float rotation) {
	OCT_ID newID;
	OCT_index newIndex;
	iOCT_camera2D* newCamera;
	iOCT_entity* entity = iOCT_entity_get(context, entityID);

	newCamera = (iOCT_camera2D*)cOCT_pool_addEntry(iOCT_pool_get(context, OCT_ECSType_camera2D), &newIndex); // registration
	newID = cOCT_IDMap_register(&context->IDMap, newIndex);

	newCamera->cameraID = newID;	// settings
	newCamera->parentID = entityID;
	newCamera->position = position;
	newCamera->zoom = zoom;
	newCamera->rotation = rotation;

	entity->cameraID = newID;		// link to parent
	entity->componentsMask |= (1ULL << OCT_ECSType_camera2D);

	return newID;
}

void OCT_camera2D_setActive(OCT_handle entityHandle) {
	iOCT_entity* entity = iOCT_entity_get(iOCT_entityContext_get(entityHandle.containerID), entityHandle.objectID);

	OCT_handle cameraHandle = {
		.containerID = entity->contextID,
		.objectID = entity->cameraID,
		.system = OCT_subsystem_ECS,
		.type = OCT_handle_camera2D
	};

	iOCT_ECSModule_instance.activeCamera = cameraHandle;
}

float OCT_camera2D_zoom(OCT_handle entityHandle, float factor) {
	assert(entityHandle.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entityHandle.containerID);
	OCT_ID cameraID = iOCT_entity_get(context, entityHandle.objectID)->cameraID;
	return iOCT_camera2D_zoom(context, cameraID, factor);
}
float iOCT_camera2D_zoom(iOCT_entityContext* context, OCT_ID cameraID, float factor) {
	iOCT_camera2D* camera = iOCT_camera2D_get(context, cameraID);
	camera->zoom = camera->zoom * (1.0f - factor) ;
	return camera->zoom;
}

#pragma region cross-module requests

OCT_mat3 _OCT_camera2D_getActiveMatrix() {
	OCT_mat3 result;
	OCT_handle activeCameraHandle = iOCT_ECSModule_instance.activeCamera;
	iOCT_entityContext* context = iOCT_entityContext_get(activeCameraHandle.containerID);
	if (activeCameraHandle.type != OCT_handle_camera2D) {
		return OCT_mat3_identity;
	}
	iOCT_camera2D camera = *iOCT_camera2D_get(context, activeCameraHandle.objectID);

	OCT_mat3 cameraTransform = OCT_mat3_generate(camera.position, (OCT_vec2) { camera.zoom, camera.zoom }, camera.rotation);
	OCT_mat3 globalPosition = iOCT_transform2D_get(context, camera.parentID)->globalMatrix;
	
	result = OCT_mat3_mul(globalPosition, cameraTransform);

	return result;
}
#pragma endregion