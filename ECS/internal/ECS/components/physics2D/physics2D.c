#include "physics2D_internal.h"
#include "ECS/types_internal.h"

#include "cOCT_EngineStructure.h"
#include "OCT_Math.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "ECS/entityContext/entityContext_internal.h"
#include "_ECS_Output/_ECS_include.h"
#include "module/ECSModule_internal.h"
#include "ECS/components/transform2D/transform2D_internal.h"
#include "ECS/components/collider2D/collider2D_internal.h"

iOCT_physics2D* iOCT_physics2D_get(iOCT_entityContext* context, OCT_ID physicsID) {
	return (iOCT_physics2D*)iOCT_getByID(context, physicsID, OCT_ECSType_physics2D);
}
void OCT_physics2D_add(OCT_handle entity, OCT_handle rigidBodyOrigin, float gravity, float mass, float inertia, float friction, float restitution, bool isStatic) {
	assert(entity.type == OCT_handle_entity);
	iOCT_physics2D_add(iOCT_entityContext_get(entity.containerID), entity.objectID, rigidBodyOrigin.objectID, gravity, mass, inertia, friction, restitution, isStatic);
}
OCT_ID iOCT_physics2D_add(iOCT_entityContext* context, OCT_ID entityID, OCT_ID rigidBodyID, float gravity, float mass, float inertia, float friction, float restitution, bool isStatic) {
	OCT_ID newID;
	OCT_index newIndex;
	iOCT_physics2D* physics;
	iOCT_entity* entity = iOCT_entity_get(context, entityID);

	cOCT_pool* pool = iOCT_pool_get(context, OCT_ECSType_physics2D);
	physics = (iOCT_physics2D*)cOCT_pool_addEntry(pool, &newIndex);
	newID = cOCT_IDMap_register(&context->entityIDMap, newIndex);

	entity->physicsID = newID;	// must update before adding rbOriginID
	iOCT_entity_updateMask(context, entityID, OCT_ECSType_physics2D);

	memset(physics, 0, sizeof(iOCT_physics2D));
	physics->parentID = entityID;
	physics->physicsID = newID;
	physics->transformID = entity->transformID;
	physics->colliderID = entity->colliderID;
	physics->hitBoxID = entity->hitBoxID;
	physics->rbOriginID = iOCT_entity_get(context, rigidBodyID)->physicsID;

	physics->mass = mass;
	physics->inertia = inertia;
	physics->friction = friction;
	physics->restitution = restitution;
	physics->gravity = gravity;
	physics->isStatic = isStatic;

	return newID;
}

#pragma region user commands
OCT_vec2 OCT_physics2D_addVelocity(OCT_handle entityHandle, OCT_vec2 velocity) {
	assert(entityHandle.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entityHandle.containerID);
	iOCT_entity* entity = iOCT_entity_get(context, entityHandle.objectID);
	return iOCT_physics2D_addVelocity(context, entity->physicsID, velocity);
}
OCT_vec2 iOCT_physics2D_addVelocity(iOCT_entityContext* context, OCT_ID physicsID, OCT_vec2 velocity) {
	iOCT_physics2D* physics = iOCT_physics2D_get(context, physicsID);
	physics->lin_v = OCT_vec2_add(physics->lin_v, velocity);
	return physics->lin_v;
}

void OCT_physics2D_setVelocity(OCT_handle entityHandle, OCT_vec2 velocity) {
	assert(entityHandle.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entityHandle.containerID);
	iOCT_entity* entity = iOCT_entity_get(context, entityHandle.objectID);
	iOCT_physics2D_setVelocity(context, entity->physicsID, velocity);
}
void iOCT_physics2D_setVelocity(iOCT_entityContext* context, OCT_ID physicsID, OCT_vec2 velocity) {
	iOCT_physics2D* physics = iOCT_physics2D_get(context, physicsID);
	physics->lin_v = velocity;
}
OCT_vec2 OCT_physics2D_readVelocity(OCT_handle entityHandle) {
	assert(entityHandle.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entityHandle.containerID);
	iOCT_entity* entity = iOCT_entity_get(context, entityHandle.objectID);
	return iOCT_physics2D_readVelocity(context, entity->physicsID);
}
OCT_vec2 iOCT_physics2D_readVelocity(iOCT_entityContext* context, OCT_ID physicsID) {
	return iOCT_physics2D_get(context, physicsID)->lin_v;
}

void OCT_physics2D_setGravity(OCT_handle entityHandle, float multiplier) {
	assert(entityHandle.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entityHandle.containerID);
	iOCT_entity* entity = iOCT_entity_get(context, entityHandle.objectID);
	iOCT_physics2D_setGravity(context, entity->physicsID, multiplier);
}
void iOCT_physics2D_setGravity(iOCT_entityContext* context, OCT_ID physicsID, float multiplier) {
	iOCT_physics2D* physics = iOCT_physics2D_get(context, physicsID);
	physics->gravity = multiplier;
}

OCT_vec2 OCT_physics2D_addForce(OCT_handle entityHandle, OCT_vec2 force) {
	assert(entityHandle.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entityHandle.containerID);
	iOCT_entity* entity = iOCT_entity_get(context, entityHandle.objectID);
	return iOCT_physics2D_addForce(context, entity->physicsID, force);
}
OCT_vec2 iOCT_physics2D_addForce(iOCT_entityContext* context, OCT_ID physicsID, OCT_vec2 force) {
	iOCT_physics2D* physics = iOCT_physics2D_get(context, physicsID);
	physics->forceNet = OCT_vec2_add(physics->forceNet, force);
	return physics->forceNet;
}
OCT_vec2 OCT_physics2D_addImpulse(OCT_handle entityHandle, OCT_vec2 impulse) {
	assert(entityHandle.type == OCT_handle_entity);
	iOCT_entityContext* context = iOCT_entityContext_get(entityHandle.containerID);
	iOCT_entity* entity = iOCT_entity_get(context, entityHandle.objectID);
	return iOCT_physics2D_addImpulse(context, entity->physicsID, impulse);
}
OCT_vec2 iOCT_physics2D_addImpulse(iOCT_entityContext* context, OCT_ID physicsID, OCT_vec2 impulse) {
	iOCT_physics2D* physics = iOCT_physics2D_get(context, physicsID);
	physics->lin_v = OCT_vec2_add(physics->lin_v, OCT_vec2_div(impulse, physics->mass));
	return physics->lin_v;
}

bool OCT_physics2D_isColliding(OCT_handle entityA, OCT_handle entityB) {
	iOCT_entityContext* context = iOCT_entityContext_get(entityA.containerID);
	OCT_ID colliderA = iOCT_entity_get(context, entityA.objectID)->colliderID;
	OCT_ID colliderB = iOCT_entity_get(context, entityB.objectID)->colliderID;
	return iOCT_physics2D_isColliding(context, colliderA, colliderB);
}
bool iOCT_physics2D_isColliding(iOCT_entityContext* context, OCT_ID colliderA, OCT_ID colliderB) {
	cOCT_message event;
	OCT_index count = 0;
	event = cOCT_event_read(OCT_subsystem_physics, count);

	while(event.messageType != cOCT_MSG_ALLCLEAR) {
		if (event.messageType == cOCT_MSG_COLLISION) {
			if ((event.collision.colliderA == colliderA && event.collision.colliderB == colliderB) ||
				(event.collision.colliderA == colliderB && event.collision.colliderB == colliderA)) {
				return true;
			}
		}
		count++;
		event = cOCT_event_read(OCT_subsystem_physics, count);
	}
	//printf("Checked %d events: all clear\n", (int)count);
	return false;
}
#pragma endregion

#pragma region cross-module
_OCT_snapshot_physics* _OCT_physics2D_packSnapshot(OCT_index* outCount, OCT_ID* outContextID, OCT_index contextIndex) {
	cOCT_pool* targetPool = &iOCT_ECSModule_instance.physicsDataPool;							// clear array
	targetPool->count = 0;
	memset(targetPool->array, 0, sizeof(_OCT_snapshot_physics) * targetPool->capacity);

	iOCT_entityContext* context = &((iOCT_entityContext*)iOCT_ECSModule_instance.contextPool.array)[contextIndex];	// get physics array
	iOCT_physics2D* physArray = (iOCT_physics2D*)iOCT_pool_get(context, OCT_ECSType_physics2D)->array;

	iOCT_physics2D* phys;
	iOCT_transform2D* transf;
	iOCT_collider2D* coll;
	iOCT_collider2D* hitb;
	OCT_index physicsCount = iOCT_pool_get(context, OCT_ECSType_physics2D)->count;
	_OCT_snapshot_physics* dataSlot;
	OCT_index dummy;
	for (OCT_index i = 0; i < physicsCount; i++) {
		dataSlot = cOCT_pool_addEntry(targetPool, &dummy);
		phys = &physArray[i];
		transf = iOCT_transform2D_get(context, phys->transformID);
		coll = iOCT_collider2D_get(context, phys->colliderID);
		hitb = iOCT_collider2D_get(context, phys->hitBoxID);

		dataSlot->physicsID = phys->physicsID;
		dataSlot->rbOriginIndex = cOCT_IDMap_getIndex(&context->entityIDMap, phys->rbOriginID);
		dataSlot->mass = phys->mass;
		dataSlot->lin_v = phys->lin_v;
		dataSlot->forceNet = phys->forceNet;
		dataSlot->inertia = phys->inertia;
		dataSlot->ang_v = phys->ang_v;
		dataSlot->torqueNet = phys->torqueNet;
		dataSlot->friction = phys->friction;
		dataSlot->restitution = phys->restitution;
		dataSlot->gravity = phys->gravity;
		dataSlot->isStatic = phys->isStatic;

		dataSlot->transformID = transf->transformID;
		dataSlot->position = transf->position;
		dataSlot->scale = transf->scale;
		dataSlot->rotation = transf->rotation;

		if (coll) {
			dataSlot->colliderID = coll->colliderID;
			dataSlot->collider = coll->shape;
		}
		if (hitb) {
			dataSlot->hitBoxID = hitb->colliderID;
			dataSlot->hitbox = hitb->shape;
		}
	}

	*outContextID = context->contextID;
	*outCount = physicsCount;
	return (_OCT_snapshot_physics*)targetPool->array;
}

void _OCT_physics2D_writeBack(OCT_index count, OCT_ID contextID) {
	iOCT_entityContext* context = iOCT_entityContext_get(contextID);
	_OCT_snapshot_physics* data = (_OCT_snapshot_physics*)iOCT_ECSModule_instance.physicsDataPool.array;

	iOCT_transform2D* transform;
	iOCT_physics2D* physics;

	_OCT_snapshot_physics* snapshot;
	for (OCT_index i = 0; i < count; i++) {
		snapshot = &data[i];

		physics = iOCT_physics2D_get(context, snapshot->physicsID);
		transform = iOCT_transform2D_get(context, snapshot->transformID);
		
		physics->lin_v = snapshot->lin_v;
		physics->forceNet = snapshot->forceNet;
		physics->ang_v = snapshot->ang_v;
		physics->torqueNet = snapshot->torqueNet;

		transform->position = snapshot->position;
		transform->rotation = snapshot->rotation;
	}
}

#pragma endregion