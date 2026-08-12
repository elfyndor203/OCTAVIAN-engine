#include "collider2D_int.h"
#include "types_int.h"

#include <box2d/box2d.h>

#include "physicsSystem_int.h"
#include "physics2D/physics2D_int.h"

OCT_local OCT_collider2D_new(OCT_local entity, OCT_shapeType shape, OCT_vec2 dimensions, OCT_vec2 position, float rotationDeg, float density) {
    b2ShapeDef newShape = b2DefaultShapeDef();
    newShape.density = density;

    OCT_mat3 globalTransform = *(OCT_mat3*)eOCT_entity_getFieldOnce(entity, iOCT_physicsSystem_inst.transform2DTicket);
    OCT_vec2 origin = OCT_vec2_add(OCT_mat3_getTranslation(globalTransform), position);
    printf("Origin in world space: %f %f\n", origin.x, origin.y);
    float rotationRad = OCT_mat3_getRotation(globalTransform) + OCT_deg2rad(rotationDeg);

    iOCT_physics2D_b2* physics = eOCT_entity_getComponentOnce(entity, iOCT_physicsSystem_inst.physics2DKey);
    b2BodyId entityBodyID = physics->b2dBodyID;

    OCT_vec2 dimensionsMeters = OCT_vec2_div(dimensions, iOCT_physicsSystem_inst.unitsPerB2Meter);
    OCT_vec2 originMeters = OCT_vec2_div(origin, iOCT_physicsSystem_inst.unitsPerB2Meter);
    printf("Origin: %f %f\n", originMeters.x, originMeters.y);
    b2ShapeId newShapeID;
    switch (shape) {
    case OCT_shapeType_rect2:
        b2Polygon newPolygon = b2MakeOffsetBox(dimensionsMeters.x / 2, dimensionsMeters.y / 2, (b2Vec2){originMeters.x, originMeters.y}, b2MakeRot(rotationRad));
        newShapeID = b2CreatePolygonShape(entityBodyID, &newShape, &newPolygon);
        break;
    case OCT_shapeType_circ2:
        if (dimensions.x != dimensions.y) {
            OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Circle x and y dimensions must be equal");
        }
        b2Circle newCircle = {
        .center = (b2Vec2){originMeters.x, originMeters.y},
        .radius = dimensionsMeters.x
        };
        newShapeID = b2CreateCircleShape(entityBodyID, &newShape, &newCircle);
        break;
    case OCT_shapeType_caps2:
        float toCentersDist = dimensionsMeters.y / 2 + dimensionsMeters.x / 2;
        OCT_vec2 toCenter1 = OCT_vec2_rotate((OCT_vec2){0, toCentersDist}, rotationRad);
        OCT_vec2 toCenter2 = OCT_vec2_rotate((OCT_vec2){0, -toCentersDist}, rotationRad);

        b2Capsule newCapsule = {
        .radius = dimensionsMeters.x / 2,
        .center1 = iOCT_toB2Vec2(toCenter1),
        .center2 = iOCT_toB2Vec2(toCenter2)
        };
        newShapeID = b2CreateCapsuleShape(entityBodyID, &newShape, &newCapsule);
        break;
    default:
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "No valid shape provided");
    }


    iOCT_collider2D newCollider = {

    };

    OCT_local colliderHandle = {
        .contextHandle = entity.contextHandle,
        .containerID = OCT_ID_NULL,
        .objectID = OCT_ID_NULL
    };
    return colliderHandle;
}