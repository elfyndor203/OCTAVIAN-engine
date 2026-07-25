#pragma once
#include "types.h"

#include "OCT_Core_eng.h"

bool OCT_transform2D_attach(OCT_handle entity, OCT_handle parentEntity);
/*!
 * Moves an entity to the given location locally (with respect to its parent)
 * @param entity
 * @param destination
 * @return Delta position
 */
OCT_vec2 OCT_transform2D_moveTo(OCT_handle entity, OCT_vec2 destination);
/*!
 * Rotates to the given angle in degrees
 * @param entity
 * @param degrees
 * @return Delta rotation
 */
float OCT_transform2D_rotateTo(OCT_handle entity, float degrees);
OCT_vec2 OCT_transform2D_read(OCT_handle entity, float* rotationOut, OCT_vec2* scaleOut);