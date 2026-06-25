#include "transform2D_internal.h"
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "OCT_Errors.h"
#include "cOCT_EngineStructure.h"
#include "OCT_Math.h"
#include <assert.h>

#include "ECS/entity/entity_internal.h"
#include "ECS/entityContext/entityContext_internal.h"

static OCT_mat3 iOCT_transform2D_generateMatrix(iOCT_transform2D* transform);
static void iOCT_transform2D_insert(iOCT_entityContext* context, iOCT_transform2D* newTransform);
static void iOCT_transform2D_pull(iOCT_entityContext* context, iOCT_transform2D* transform);
static void iOCT_transform2D_updateParentCaches(iOCT_entityContext* context);

iOCT_transform2D* iOCT_transform2D_get(iOCT_entityContext* context, OCT_ID transformID) {
    return iOCT_getByID(context, transformID, OCT_ECSType_transform2D);
}

/// <summary>
/// Attaches a new transform2D component to the chosen entity. Returns false if anything failed.
/// </summary>
/// <param name="parentHandle"></param>
/// <returns></returns>
bool OCT_transform2D_add(OCT_handle handle) {
    assert(handle.type == OCT_handle_entity);

    iOCT_entityContext* context = iOCT_entityContext_get(handle.containerID);
    if (iOCT_transform2D_add(context, handle.objectID) == OCT_ID_NULL) {
        return false;
    }
    return true;
}
OCT_ID iOCT_transform2D_add(iOCT_entityContext* context, OCT_ID entityID) {
    OCT_ID newID;
    OCT_index newIndex;
    iOCT_transform2D* newTransform;
    iOCT_entity* entity = iOCT_entity_get(context, entityID);
    iOCT_transform2D* parentTransform;
    OCT_ID parentID;
    OCT_index parentIndex;

    int parentDepth;
    if (entityID == iOCT_ROOT_ID) {     // if root object
        parentDepth = iOCT_TRANSFORM_ROOTDEPTH - 1;
        parentIndex = OCT_index_NULL;
        parentID = OCT_ID_NULL;
    }
    else {
        parentTransform = iOCT_transform2D_get(context, iOCT_entity_get(context, iOCT_entity_get(context, entityID)->parentID)->transformID);
        parentDepth = parentTransform->depth;
        parentIndex = cOCT_IDMap_getIndex(&context->entityIDMap, parentTransform->transformID);
        parentID = parentTransform->transformID;
    }

    newTransform = (iOCT_transform2D*)cOCT_pool_addEntry(iOCT_pool_get(context, OCT_ECSType_transform2D), &newIndex);
    newID = cOCT_IDMap_register(&context->entityIDMap, newIndex);
    memset(newTransform, 0, sizeof(iOCT_transform2D));

    // Set values
    newTransform->transformID = newID;
    newTransform->entityID = entityID;
    newTransform->parentID = parentID;
    newTransform->parentCache = parentIndex;
    newTransform->depth = parentDepth + 1;
    newTransform->position = (OCT_vec2){iOCT_DEFAULT_POSITION_X, iOCT_DEFAULT_POSITION_Y};
    newTransform->rotation = iOCT_DEFAULT_ROTATION;
    newTransform->scale = (OCT_vec2){iOCT_DEFAULT_SCALE_X, iOCT_DEFAULT_SCALE_Y};
	newTransform->globalMatrix = OCT_mat3_identity;

    if (newTransform->depth >= iOCT_TRANSFORM_MAXDEPTH) {
        OCT_logError(EXIT_TRANSFORM2D_MAX_DEPTH_EXCEEDED);
        return OCT_ID_NULL;
    }

    // Link to parent
    entity->transformID = newID;
    iOCT_entity_updateMask(context, entityID, OCT_ECSType_transform2D);

    // maintain depth sort
    iOCT_transform2D_insert(context, newTransform);

	printf("Current max depth: %d\n", context->currentMaxDepth);
    return newID;
}

void iOCT_transform2D_delete(iOCT_entityContext* context, OCT_ID entityID) {
	iOCT_transform2D* transform = iOCT_transform2D_get(context, iOCT_entity_get(context, entityID)->transformID);
    iOCT_transform2D_pull(context, transform);
}

void iOCT_transform2D_propagate(iOCT_entityContext* context) {
    iOCT_transform2D_updateParentCaches(context);

    cOCT_pool* pool = iOCT_pool_get(context, OCT_ECSType_transform2D);
    iOCT_transform2D* array = (iOCT_transform2D*)pool->array;

    iOCT_transform2D* currentTransform;
    iOCT_transform2D* parentTransform;

    OCT_index rootIndex = cOCT_IDMap_getIndex(&context->entityIDMap, iOCT_entity_get(context, iOCT_ROOT_ID)->transformID);
    currentTransform = &array[rootIndex];
    currentTransform->localMatrix = iOCT_transform2D_generateMatrix(currentTransform);
    currentTransform->globalMatrix = currentTransform->localMatrix;

    for (OCT_index index = rootIndex + 1; index < pool->count; index++) {
        currentTransform = &array[index];
        
        assert(currentTransform->parentCache != OCT_index_NULL);
        parentTransform = &array[currentTransform->parentCache];

        currentTransform->localMatrix = iOCT_transform2D_generateMatrix(currentTransform);
        currentTransform->globalMatrix = OCT_mat3_mul(parentTransform->globalMatrix, currentTransform->localMatrix);
    }
}

#pragma region helpers

static void iOCT_transform2D_insert(iOCT_entityContext* context, iOCT_transform2D* transform) {
    iOCT_transform2D newTransform = *transform; // do not work with the existing memory in the pool

    int* currentMaxDepth = &context->currentMaxDepth;
    OCT_index* depthEnds = context->depthEnds;
    int newDepth = newTransform.depth;

    iOCT_transform2D* poolArray = (iOCT_transform2D*)iOCT_pool_get(context, OCT_ECSType_transform2D)->array;
    OCT_index targetIndex;

    int workingDepth = newDepth;
    iOCT_transform2D workingTransform = newTransform;
    iOCT_transform2D displacedTransform;

    if (newDepth == *currentMaxDepth) {  // insert at the end, nothing gets displaced, nothing needs resolving
        depthEnds[newDepth] += 1;
        poolArray[depthEnds[newDepth]] = newTransform;  // replace anyway
        cOCT_IDMap_remap(&context->entityIDMap, newTransform.transformID, depthEnds[newDepth]);
        return;
    }
    if (newDepth > *currentMaxDepth) {   // insert at the end, nothing gets displaced, nothing needs resolving
        if (*currentMaxDepth < 0) {
            depthEnds[newDepth] = 0;
        }
        else {
            depthEnds[newDepth] = depthEnds[*currentMaxDepth] + 1;
        }
        poolArray[depthEnds[newDepth]] = newTransform;  // replace anyway
        cOCT_IDMap_remap(&context->entityIDMap, newTransform.transformID, depthEnds[newDepth]);
        *currentMaxDepth = newDepth;
        return;
    }
    while (workingDepth < *currentMaxDepth) {
        targetIndex = depthEnds[workingDepth] + 1;      // start of the next depth
        displacedTransform = poolArray[targetIndex];    // temp copy of the displaced

        poolArray[targetIndex] = workingTransform;      // copy the new one in
        cOCT_IDMap_remap(&context->entityIDMap, workingTransform.transformID, targetIndex);

        workingTransform = displacedTransform;          // start doing the same to the next layer
        depthEnds[workingDepth] += 1;                   // expand the depth group by 1
        workingDepth += 1;
    }
    OCT_index finalIndex = depthEnds[*currentMaxDepth] + 1;
    poolArray[finalIndex] = workingTransform;
    cOCT_IDMap_remap(&context->entityIDMap, workingTransform.transformID, finalIndex);
    depthEnds[*currentMaxDepth] += 1;
}

static void iOCT_transform2D_pull(iOCT_entityContext* context, iOCT_transform2D* transform) {
	cOCT_pool* pool = iOCT_pool_get(context, OCT_ECSType_transform2D);
    cOCT_IDMap* map = &context->entityIDMap;

    iOCT_transform2D* keep;                // last transform in the next layer
    iOCT_transform2D* replace = transform;    // hole to be filled with keep
    OCT_index keepIndex;                                                            // location of keep
    OCT_index replaceIndex = cOCT_IDMap_getIndex(map, transform->transformID);      // destination of keep

    cOCT_pool_deleteEntry(pool, replaceIndex, false); // remove and decrement count but do not reshuffle

    for (int layer = transform->depth; layer < context->currentMaxDepth; layer++) { // layer is the depth of the hole, so the final depth does not need handling
		keepIndex = context->depthEnds[layer + 1]; // index of the last transform in the next layer, which will be moved up to fill the hole
        keep = cOCT_pool_access(pool, keepIndex);

		memcpy(replace, keep, sizeof(iOCT_transform2D)); // replace data

		context->depthEnds[layer + 1] -= 1; 
		cOCT_IDMap_remap(map, keep->transformID, replaceIndex); // remap to its new destination

        replace = keep;           // prep for next layer
		replaceIndex = keepIndex; // next hole to be filled is the one left by the moved transform
    }
}

static OCT_mat3 iOCT_transform2D_generateMatrix(iOCT_transform2D* transform) {
    float cos = cosf(transform->rotation);
    float sin = sinf(transform->rotation);
    float scaleX = transform->scale.x;
    float scaleY = transform->scale.y;

    OCT_mat3 localMatrix = {
        scaleX * cos,  scaleX * sin,  0,   // column 0
       -scaleY * sin,  scaleY * cos,  0,   // column 1
        transform->position.x, transform->position.y, 1  // column 2
    };
    return localMatrix;
}

static void iOCT_transform2D_updateParentCaches(iOCT_entityContext* context) {
    cOCT_pool* transformPool = iOCT_pool_get(context, OCT_ECSType_transform2D);
    iOCT_transform2D* array = (iOCT_transform2D*)transformPool->array;

    iOCT_transform2D* transform;
    OCT_index newIndex;
    for (int i = 0; i < transformPool->count; i++) {
        transform = &array[i];
        newIndex = cOCT_IDMap_getIndex(&context->entityIDMap, transform->parentID);
        transform->parentCache = newIndex;
    }
}

#pragma endregion

OCT_vec2 iOCT_transform2D_globalPos(iOCT_transform2D transform) {
    OCT_mat3 matrix = transform.globalMatrix;
    OCT_vec2 globalPos = {
        .x = matrix.c2r0,
        .y = matrix.c2r1
    };
    return globalPos;
}

#pragma region User Commands

/// <summary>
/// Moves an entity by a given vector using its transform component.
/// </summary>
/// <param name="parentHandle"></param>
/// <param name="delta"></param>
/// <returns></returns>
OCT_vec2 OCT_transform2D_moveBy(OCT_handle entity, OCT_vec2 delta) {
    assert(entity.type == OCT_handle_entity);

    iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);
    OCT_ID transformID = iOCT_entity_get(context, entity.objectID)->transformID;
    return iOCT_transform2D_moveBy(context, transformID, delta);
}
OCT_vec2 iOCT_transform2D_moveBy(iOCT_entityContext* context, OCT_ID transformID, OCT_vec2 delta) {
    iOCT_transform2D* transform = iOCT_transform2D_get(context, transformID);
    transform->position = OCT_vec2_add(transform->position, delta);
    // printf("MOV transform2D %5" PRIu64 " to (%.4f, %.4f) in entityContext %" PRIu64 "\n", transformID, transform->position.x, transform->position.y, entityContextID);
    return transform->position;
}

/// <summary>
/// Moves an entity to a given local position.
/// </summary>
/// <param name="parentHandle"></param>
/// <param name="delta"></param>
/// <returns></returns>
OCT_vec2 OCT_transform2D_moveTo(OCT_handle entity, OCT_vec2 delta) {
    assert(entity.type == OCT_handle_entity);

    iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);
    OCT_ID transformID = iOCT_entity_get(context, entity.objectID)->transformID;
    return iOCT_transform2D_moveTo(context, transformID, delta);
}
OCT_vec2 iOCT_transform2D_moveTo(iOCT_entityContext* context, OCT_ID transformID, OCT_vec2 newPos) {
    iOCT_transform2D* transform = iOCT_transform2D_get(context, transformID);
    transform->position = newPos;
    return transform->position;
}

/// <summary>
/// Rotates an entity by a given angle in degrees using its transform component. 
/// </summary>
/// <param name="parentHandle"></param>
/// <param name="deltaDegrees"></param>
/// <returns></returns>
float OCT_transform2D_rotateByDeg(OCT_handle entity, float deltaDegrees) {
    assert(entity.type == OCT_handle_entity);

    iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);
    OCT_ID transformID = iOCT_entity_get(context, entity.objectID)->transformID;

    return OCT_rad2deg(iOCT_transform2D_rotateBy(context, transformID, OCT_deg2rad(deltaDegrees)));
}
/// <summary>
/// Rotates an entity by a given angle in radians using its transform component.
/// </summary>
/// <param name="parentHandle"></param>
/// <param name="deltaRadians"></param>
/// <returns></returns>
float OCT_transform2D_rotateByRad(OCT_handle entity, float deltaRadians) {
    assert(entity.type == OCT_handle_entity);

    iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);
    OCT_ID transformID = iOCT_entity_get(context, entity.objectID)->transformID;
    return iOCT_transform2D_rotateBy(context, transformID, deltaRadians);
}
float iOCT_transform2D_rotateBy(iOCT_entityContext* context, OCT_ID transformID, float deltaRad) {
    iOCT_transform2D* transform = iOCT_transform2D_get(context, transformID);
    transform->rotation += deltaRad;  //NOTE_FOR_CCW_UNSURE_WHY

    transform->rotation = fmodf(transform->rotation, OCT_2PI_f);  // normalize to [0, 2pi]
    if (transform->rotation < 0) {
        transform->rotation += OCT_2PI_f;
    }

    return transform->rotation;
}

OCT_vec2 OCT_transform2D_scaleBy(OCT_handle entity, OCT_vec2 delta) {
    assert(entity.type == OCT_handle_entity);

    iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);
    OCT_ID transformID = iOCT_entity_get(context, entity.objectID)->transformID;
    return iOCT_transform2D_scaleBy(context, transformID, delta);
}
OCT_vec2 iOCT_transform2D_scaleBy(iOCT_entityContext* context, OCT_ID transformID, OCT_vec2 delta) {
    iOCT_transform2D* transform = iOCT_transform2D_get(context, transformID);
    transform->scale.x *= delta.x;
    transform->scale.y *= delta.y;
    return transform->scale;
}

OCT_vec2 OCT_transform2D_readPos(OCT_handle entity) {
    assert(entity.type == OCT_handle_entity);

    iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);
    OCT_ID transformID = iOCT_entity_get(context, entity.objectID)->transformID;
    return iOCT_transform2D_readPos(context, transformID);
}
OCT_vec2 iOCT_transform2D_readPos(iOCT_entityContext* context, OCT_ID transformID) {
    return iOCT_transform2D_get(context, transformID)->position;
}

OCT_vec2 OCT_transform2D_readPosGlobal(OCT_handle entity) {
    assert(entity.type == OCT_handle_entity);

    iOCT_entityContext* context = iOCT_entityContext_get(entity.containerID);
    OCT_ID transformID = iOCT_entity_get(context, entity.objectID)->transformID;
    return iOCT_transform2D_readPosGlobal(context, transformID);
}
OCT_vec2 iOCT_transform2D_readPosGlobal(iOCT_entityContext* context, OCT_ID transformID) {
    OCT_mat3 transform = iOCT_transform2D_get(context, transformID)->globalMatrix;
    return (OCT_vec2) { transform.c2r0, transform.c2r1 };
}

#pragma endregion

#pragma region cross-module requests

#pragma endregion
