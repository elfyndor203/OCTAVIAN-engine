#pragma once
#include "types.h"

#include "OCT_Core_eng.h"

bool OCT_transform2D_attach(OCT_local entity, OCT_local parentEntity);
/*!
 * Moves an entity to the given location locally (with respect to its parent)
 * @param entity
 * @param destination
 * @return Delta position
 */
OCT_vec2 OCT_transform2D_moveTo(OCT_local entity, OCT_vec2 destination);
OCT_vec2 OCT_transform2D_moveBy(OCT_local entity, OCT_vec2 deltaXY);
/*!
 * Rotates to the given angle in degrees
 * @param entity
 * @param radians
 * @return Delta rotation
 */
float OCT_transform2D_rotateTo(OCT_local entity, float radians);
OCT_vec2 OCT_transform2D_read(OCT_local entity, float* rotationOut, OCT_vec2* scaleOut);