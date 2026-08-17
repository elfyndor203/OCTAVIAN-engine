#pragma once
#include "math/vectors/vec2.h"
#include "utilities/utilities.h"
#include "math/lines/line2.h"

typedef struct {
	OCT_vec2 center;
	OCT_vec2 dimensions;
	float rotationRad;
} OCT_rect2;

/*!
 * Returns the top right (pos, pos) vertex coordinate, and the rest optionally.
 * @param rect
 * @param topL
 * @param botL
 * @param botR
 * @return topR
 */
OCT_vec2 OCT_rect2_vertices(OCT_rect2 rect, OCT_vec2* topL, OCT_vec2* botL, OCT_vec2* botR);

/// <summary> Returns the width normal and the height normal optionally. </summary>/// <param name="rect"></param>/// <param name="heightAxis"></param>/// <returns></returns>
OCT_vec2 OCT_rect2_normals(OCT_rect2 rect, OCT_vec2* heightAxis);
OCT_segment2 OCT_rect2_edges(OCT_rect2 rect, OCT_segment2* leftOut, OCT_segment2* bottomOut, OCT_segment2* rightOut);
OCT_vec2 OCT_rect2_SAT(OCT_rect2 rectA, OCT_rect2 rectB, OCT_vec2* axisOut, float* overlapOut, OCT_AorB* referenceRectOut);
OCT_AorB OCT_rect2_contacts(OCT_rect2 referenceRectA, OCT_rect2 rectB, OCT_vec2 MTV, OCT_vec2 minAxis, float minOverlap, OCT_vec2* contactAOut, OCT_vec2* contactBOut);