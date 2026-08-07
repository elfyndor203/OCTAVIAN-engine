#include "collisions_internal.h"
#include "physics/types_internal.h"

#include "cOCT_Communication.h"
#include "cOCT_EngineStructure.h"
#include "OCT_Math.h"
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <stdio.h>

#include "physics/physics.h"
#include "module/PHYModule_internal.h"

static OCT_vec2 iOCT_physics_project(OCT_vec2 axis, OCT_vec2* verts, OCT_index count);
static float iOCT_physics_projOverlap(OCT_vec2 projA, OCT_vec2 projB);
static bool iOCT_physics_broad2Rect2(OCT_rect2 rectA, OCT_rect2 rectB);
static OCT_vec2 iOCT_physics_SAT2Rect2(OCT_rect2 rectA, OCT_rect2 rectB);

static void iOCT_physics_resolveImpulse(_OCT_snapshot_physics* physA, _OCT_snapshot_physics* physB, iOCT_collision collision);
static void iOCT_physics_resolvePosition(_OCT_snapshot_physics* physA, _OCT_snapshot_physics* physB, iOCT_collision collision);

iOCT_collision iOCT_collision_none = { OCT_handle_NULL, OCT_handle_NULL, 0.0f, 0.0f };

#pragma region detection
iOCT_collision iOCT_physics_detectCollision(_OCT_snapshot_physics* physA, _OCT_snapshot_physics* physB) {
	iOCT_collision collision = iOCT_collision_none;
	float radiusA;
	float radiusB;
	OCT_rect2 rectA;
	OCT_rect2 rectB;

	if (physA->rbOriginIndex == physB->rbOriginIndex) {	// if they belong to the same rigidbody
		return iOCT_collision_none;
	}

	if (physA->collider.type == OCT_shapeType_rect2 && physB->collider.type == OCT_shapeType_rect2) {
		rectA = physA->collider.rect2;
		rectB = physB->collider.rect2;
		rectA.origin = OCT_vec2_add(rectA.origin, physA->position);
		rectB.origin = OCT_vec2_add(rectB.origin, physB->position);
		if (!iOCT_physics_broad2Rect2(rectA, rectB)) {
			return iOCT_collision_none;
		}

		collision.colliderA = physA->colliderID;
		collision.colliderB = physB->colliderID;
		collision.MTV = iOCT_physics_SAT2Rect2(rectA, rectB);
	}

	if (OCT_vec2_equal(collision.MTV, OCT_vec2_zero, OCT_FLOAT_EQUAL_EPSILON)) {
		return iOCT_collision_none;
	}
	else {
		cOCT_message collMsg = {
			.messageType = cOCT_MSG_COLLISION,
			.collision = {collision.colliderA, collision.colliderB}
		};
		cOCT_message_push(OCT_subsystem_physics, collMsg, cOCT_EVENTBOX);
		//printf("MTV: %f %f\n", collision.MTV.x, collision.MTV.y);
		return collision;
	}
}

static bool iOCT_physics_broad2Rect2(OCT_rect2 rectA, OCT_rect2 rectB) {
	float radiusA;
	float radiusB;

	radiusA = sqrtf((rectA.width / 2) * (rectA.width / 2) + (rectA.height / 2) * (rectA.height / 2));
	radiusB = sqrtf((rectB.width / 2) * (rectB.width / 2) + (rectB.height / 2) * (rectB.height / 2));

	if (OCT_vec2_mag(OCT_vec2_sub(rectA.origin, rectB.origin)) > (radiusA + radiusB)) {	// too far apart
		//printf("Broad dist: %f\n", OCT_vec2_mag(OCT_vec2_sub(rectA.origin, rectB.origin)));
		return false;
	}
	else {
		return true;
	}
}

static OCT_vec2 iOCT_physics_SAT2Rect2(OCT_rect2 rectA, OCT_rect2 rectB) {
	OCT_vec2 vertsA[4];	// vertices to project
	OCT_vec2 vertsB[4];
	vertsA[0] = OCT_rect2_getVerts(rectA, &vertsA[1], &vertsA[2], &vertsA[3]);
	vertsB[0] = OCT_rect2_getVerts(rectB, &vertsB[1], &vertsB[2], &vertsB[3]);

	OCT_vec2 axesA[2];	// projection axes
	OCT_vec2 axesB[2];
	axesA[0] = OCT_rect2_getNormals(rectA, &axesA[1]);
	axesB[0] = OCT_rect2_getNormals(rectB, &axesB[1]);

	OCT_vec2 projA;
	OCT_vec2 projB;
	float overlap;
	float minOverlap = FLT_MAX;
	OCT_vec2 minAxis = OCT_vec2_zero; // guaranteed to be set at some point
	OCT_vec2 MTV;
	for (int axisA = 0; axisA < 2; axisA++) {		// for each axis of A
		projA = iOCT_physics_project(axesA[axisA], vertsA, 4);
		projB = iOCT_physics_project(axesA[axisA], vertsB, 4);
		overlap = iOCT_physics_projOverlap(projA, projB);
		if (overlap <= 0) {			// if no overlap, no collision
			return OCT_vec2_zero;
		}
		if (overlap < minOverlap) {
			minOverlap = overlap;
			minAxis = axesA[axisA];
		}
	}
	for (int axisB = 0; axisB < 2; axisB++) {		// for each axis of B
		projA = iOCT_physics_project(axesB[axisB], vertsA, 4);
		projB = iOCT_physics_project(axesB[axisB], vertsB, 4);
		overlap = iOCT_physics_projOverlap(projA, projB);
		if (overlap <= 0) {
			return OCT_vec2_zero;
		}
		if (overlap < minOverlap) {
			minOverlap = overlap;
			minAxis = axesB[axisB];
		}
	}

	OCT_vec2 direction = OCT_vec2_sub(rectA.origin, rectB.origin);
	if (OCT_vec2_dot(direction, minAxis) < 0) {
		minAxis = OCT_vec2_neg(minAxis);
	}
	MTV = OCT_vec2_mul(minAxis, minOverlap);
	return MTV;
}
#pragma endregion

#pragma region resolution

void iOCT_physics_resolveCollision(_OCT_snapshot_physics* physA, _OCT_snapshot_physics* physB, iOCT_collision collision) {
	iOCT_physics_resolveImpulse(physA, physB, collision);
	iOCT_physics_resolvePosition(physA, physB, collision);
}

static void iOCT_physics_resolveImpulse(_OCT_snapshot_physics* physA, _OCT_snapshot_physics* physB, iOCT_collision collision) {
	OCT_vec2 midpoint = OCT_vec2_div(OCT_vec2_add(physA->position, physB->position), 2.0f);	// estimate contact pt, NOTE_
	OCT_vec2 contact = OCT_vec2_add(midpoint, OCT_vec2_div(collision.MTV, 2.0f));
	OCT_vec2 rA = OCT_vec2_sub(contact, physA->position);	// A to contact
	OCT_vec2 rB = OCT_vec2_sub(contact, physB->position);	// B to contact

	OCT_vec2 relA = OCT_vec2_add(physA->lin_v, (OCT_vec2) { -physA->ang_v * rA.y, physA->ang_v* rA.x });	// linear velocity of point
	OCT_vec2 relB = OCT_vec2_add(physB->lin_v, (OCT_vec2) { -physB->ang_v * rB.y, physB->ang_v* rB.x });
	OCT_vec2 relV = OCT_vec2_sub(relA, relB);

	OCT_vec2 normal = OCT_vec2_norm(collision.MTV);
	float rAxN = OCT_vec2_cross(rA, normal);
	float rBxN = OCT_vec2_cross(rB, normal);
	float relVNorm = OCT_vec2_dot(relV, normal);
	if (relVNorm > 0) { // already separating NOTE_
		return;
	}

	float restitution = 1.0f + ((physA->restitution + physB->restitution) * 0.5f);
	float invMassA = physA->isStatic ? 0.0f : 1.0f / physA->mass;
	float invMassB = physB->isStatic ? 0.0f : 1.0f / physB->mass;
	float invMass = invMassA + invMassB;
	float invInertiaA = physA->isStatic ? 0.0f : 1.0f / physA->inertia;
	float invInertiaB = physB->isStatic ? 0.0f : 1.0f / physB->inertia;
	float angular = 0;// ((powf(rAxN, 2))* invInertiaA) + (powf(rBxN, 2) * invInertiaB); NOTE_UNCOMMENT WHEN CONTACT POINT IS FIXED

	float impulse = -(restitution * relVNorm) / (invMass + angular);
	

	if (!physA->isStatic) {
		physA->lin_v = OCT_vec2_add(physA->lin_v, OCT_vec2_mul(normal, impulse / physA->mass));
		//printf("Impulse: %f | lin_v add: %f %f\n", impulse, OCT_vec2_mul(normal, impulse / physA->mass).x, OCT_vec2_mul(normal, impulse / physA->mass).y);
	}
	if (!physB->isStatic) {
		physB->lin_v = OCT_vec2_sub(physB->lin_v, OCT_vec2_mul(normal, impulse / physB->mass));
	}

	//physA->ang_v += OCT_vec2_cross(rA, normal) * (impulse / physA->inertia); spinning wildly, probably contact point
	//physB->ang_v -= OCT_vec2_cross(rB, normal) * (impulse / physB->inertia);
}

static void iOCT_physics_resolvePosition(_OCT_snapshot_physics* physA, _OCT_snapshot_physics* physB, iOCT_collision collision) {
	if (!physA->isStatic && physB->isStatic) {	// one static
		physA->position = OCT_vec2_add(physA->position, collision.MTV);
	}
	else if (physA->isStatic && !physB->isStatic) {	// one static
		physB->position = OCT_vec2_sub(physB->position, collision.MTV);
	}
	else {
		float totalInvMass = 1.0f / physA->mass + 1.0f / physB->mass;
		physA->position = OCT_vec2_add(physA->position, OCT_vec2_mul(collision.MTV, (1.0f / physA->mass) / totalInvMass));
		physB->position = OCT_vec2_sub(physB->position, OCT_vec2_mul(collision.MTV, (1.0f / physB->mass) / totalInvMass));
	}
	//printf("After A pos: %f %f\n\n", physA->position.x, physA->position.y);
}
#pragma endregion
#pragma region helpers
static OCT_vec2 iOCT_physics_project(OCT_vec2 axis, OCT_vec2* verts, OCT_index count) {
	float min = OCT_vec2_dot(axis, verts[0]);
	float max = min;
	
	float proj;
	for (int v = 1; v < count; v++) {
		proj = OCT_vec2_dot(axis, verts[v]);
		if (proj < min) {
			min = proj;
		}
		else if (proj > max) {
			max = proj;
		}
	}

	return (OCT_vec2) { min, max };
}

static float iOCT_physics_projOverlap(OCT_vec2 projA, OCT_vec2 projB) {
	float overlap = fminf(projA.y, projB.y) - fmaxf(projA.x, projB.x);
	if (overlap <= 0) {
		return 0;
	}
	else {
		return overlap;
	}
}
#pragma endregion
