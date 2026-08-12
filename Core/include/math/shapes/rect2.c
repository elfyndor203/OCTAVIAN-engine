#include "shapes.h"

#include "math/vectors/vec2.h"
#include "math/units/constants.h"
#include <math.h>
#include <float.h>

#include "errors/errors_eng.h"
#include "math/definitions/defaultValues.h"
#include "utilities/utilities.h"
#include "math/vectors/vec4.h"
#include "math/lines/line2.h"

OCT_vec2 OCT_rect2_vertices(OCT_rect2 rect, OCT_vec2* topL, OCT_vec2* botL, OCT_vec2* botR) {
    OCT_vec2 halfDims = OCT_vec2_div(rect.dimensions, 2);
    OCT_vec2 toTopR = OCT_vec2_rotate(halfDims, rect.rotationRad);
    OCT_vec2 topR = OCT_vec2_add(toTopR, rect.center);

    if (topL) {
        *topL = OCT_vec2_add(OCT_vec2_rotate((OCT_vec2){-halfDims.x, halfDims.y}, rect.rotationRad), rect.center);
    }
    if (botL) {
        *botL = OCT_vec2_add(OCT_vec2_rotate((OCT_vec2){-halfDims.x, -halfDims.y}, rect.rotationRad), rect.center);
    }
    if (botR) {
        *botR = OCT_vec2_add(OCT_vec2_rotate((OCT_vec2){halfDims.x, -halfDims.y}, rect.rotationRad), rect.center);
    }

    return topR;
}

OCT_vec2 OCT_rect2_normals(OCT_rect2 rect, OCT_vec2* heightAxis) {
    OCT_vec2 widthAxis = OCT_vec2_rotate((OCT_vec2) { 1, 0 }, rect.rotationRad);
    if (heightAxis) {
        *heightAxis = (OCT_vec2){ -widthAxis.y, widthAxis.x };
    }
    return widthAxis;
}

OCT_segment2 OCT_rect2_edges(OCT_rect2 rect, OCT_segment2* leftOut, OCT_segment2* bottomOut, OCT_segment2* rightOut) {
    OCT_vec2 topR;
    OCT_vec2 topL;
    OCT_vec2 bottomL;
    OCT_vec2 bottomR;
    topR = OCT_rect2_vertices(rect, &topL, &bottomL, &bottomR);

    OCT_segment2 top = {topR, topL};
    if (leftOut) {
        *leftOut = (OCT_segment2){topL, bottomL};
    }
    if (bottomOut) {
        *bottomOut = (OCT_segment2){bottomL, bottomR};
    }
    if (rightOut) {
        *rightOut = (OCT_segment2) {bottomR, topR};
    }
    return top;
}

OCT_vec2 OCT_rect2_SAT(OCT_rect2 rectA, OCT_rect2 rectB, OCT_vec2* axisOut, float* overlapOut, OCT_AorB* referenceRectOut) {
    OCT_vec2 vertsA[4];	// vertices to project
    OCT_vec2 vertsB[4];
    vertsA[0] = OCT_rect2_vertices(rectA, &vertsA[1], &vertsA[2], &vertsA[3]);
    vertsB[0] = OCT_rect2_vertices(rectB, &vertsB[1], &vertsB[2], &vertsB[3]);

    OCT_vec2 axesA[2];	// projection axes
    OCT_vec2 axesB[2];
    axesA[0] = OCT_rect2_normals(rectA, &axesA[1]);
    axesB[0] = OCT_rect2_normals(rectB, &axesB[1]);

    OCT_vec2 projA;     // __NOTE__ track full containment here, later
    OCT_vec2 projB;
    float overlap;
    float minOverlap = FLT_MAX;
    OCT_vec2 minAxis = OCT_VEC2_ZERO; // guaranteed to be set at some point
    OCT_vec2 MTV;
    OCT_AorB minAorB = OCT_NEITHER;
    for (int axisOfA = 0; axisOfA < 2; axisOfA++) {		// for each axis of A
        projA = OCT_vec2_projVertices(axesA[axisOfA], vertsA, 4);
        projB = OCT_vec2_projVertices(axesA[axisOfA], vertsB, 4);
        overlap = OCT_vec2_overlap(projA, projB);
        if (overlap <= 0) {			// if no overlap, no collision
            return OCT_VEC2_ZERO;
        }
        if (overlap < minOverlap) {
            minOverlap = overlap;
            minAxis = axesA[axisOfA];
            minAorB = OCT_A;
        }
    }
    for (int axisOfB = 0; axisOfB < 2; axisOfB++) {		// for each axis of B
        projA = OCT_vec2_projVertices(axesB[axisOfB], vertsA, 4);
        projB = OCT_vec2_projVertices(axesB[axisOfB], vertsB, 4);
        overlap = OCT_vec2_overlap(projA, projB);
        if (overlap < -OCT_FLOAT_EPSILON) {      // __NOTE__ grazing collisions report?
            return OCT_VEC2_ZERO;
        }
        if (overlap < minOverlap) {
            minOverlap = overlap;
            minAxis = axesB[axisOfB];
            minAorB = OCT_B;
        }
    }

    OCT_vec2 direction = OCT_vec2_sub(rectB.center, rectA.center);
    if (OCT_vec2_dot(direction, minAxis) < 0) {
        minAxis = OCT_vec2_neg(minAxis);
    }
    MTV = OCT_vec2_mul(minAxis, minOverlap);

    if (axisOut) {
        if (minAorB == OCT_A) {
            *axisOut = minAxis;
        }
        else {
            *axisOut = OCT_vec2_neg(minAxis);
        }
    }
    if (overlapOut) {
        *overlapOut = minOverlap;
    }
    if (referenceRectOut) {
        *referenceRectOut = minAorB;
    }
    return MTV;
}

OCT_AorB OCT_rect2_contacts(OCT_rect2 referenceRectA, OCT_rect2 rectB, OCT_vec2 MTV, OCT_vec2 minAxis, float minOverlap, OCT_vec2* contactAOut, OCT_vec2* contactBOut) {
    if (OCT_vec2_equal(MTV, OCT_VEC2_ZERO, 0)) {
        if (contactBOut) {
            *contactBOut = OCT_VEC2_NULL;
        }
        return OCT_NEITHER;
    }
    // finds the reference edge from the data provided by SAT
    OCT_segment2 refEdge = OCT_SEGMENT_NULL;
    OCT_line2 refSideA;
    OCT_line2 refSideB;
    OCT_segment2 refEdges[4];
    refEdges[0] = OCT_rect2_edges(referenceRectA, &refEdges[1], &refEdges[2], &refEdges[3]);
    bool refFound = false;
    for (OCT_index refEdgeCtr = 0; refEdgeCtr < 4; refEdgeCtr++) {
        OCT_segment2 edge = refEdges[refEdgeCtr];
        OCT_vec2 normal = OCT_vec2_perp((OCT_vec2_sub(edge.end, edge.start)), OCT_B);
        float dot = OCT_vec2_dot(normal, minAxis);
        if (1 - dot < OCT_FLOAT_EPSILON) {
            refEdge = edge;
            refFound = true;
        }
    }
    if (!refFound) {
        OCT_ERROR_LOG(OCT_EXIT_INVALID_ARGUMENT, "Could not find reference edge with given axis");
        return OCT_NEITHER;
    }
    OCT_vec2 refEdgeUnit = OCT_vec2_unit((OCT_vec2_sub(refEdge.end, refEdge.start)));
    OCT_vec2 negRefEdgeUnit = OCT_vec2_neg(refEdgeUnit);
    refSideA.normal = negRefEdgeUnit;
    refSideA.offset = OCT_vec2_dot(negRefEdgeUnit, refEdge.start);
    refSideB.normal = refEdgeUnit;
    refSideB.offset = OCT_vec2_dot(refEdgeUnit, refEdge.end);

    // finds the edge on the 2nd rectangle that most faces the reference edge
    OCT_segment2 incidentEdge = OCT_SEGMENT_NULL;
    OCT_segment2 edgesB[4];
    edgesB[0] = OCT_rect2_edges(rectB, &edgesB[1], &edgesB[2], &edgesB[3]);
    float minDot = FLT_MAX;
    for (OCT_index edgeBCtr = 0; edgeBCtr < 4; edgeBCtr++) {
        OCT_segment2 edge = edgesB[edgeBCtr];
        OCT_vec2 normal = OCT_vec2_perp((OCT_vec2_sub(edge.end, edge.start)), OCT_B);
        float dot = OCT_vec2_dot(normal, minAxis);
        if (dot < minDot) {
            incidentEdge = edge;
            minDot = dot;
        }
    }

    // clips the parts of the incident edge that fall outside the sides of the reference rectangle
    OCT_segment2 sideHalfClipped = OCT_line2_clip(refSideA, incidentEdge);
    OCT_segment2 sideClipped = OCT_line2_clip(refSideB, sideHalfClipped);

    // clip parts of the incident edge that fall outside the reference face of the ref rect
    OCT_vec2 refNormal = minAxis;
    OCT_line2 refFace = {
        .normal = refNormal,
        .offset = OCT_vec2_dot(refNormal, refEdge.start)
    };

    OCT_AorB valids = OCT_BOTH;
    OCT_vec2 contact1 = sideClipped.start;
    OCT_vec2 contact2 = sideClipped.end;
    if (!OCT_vec2_equal(sideClipped.start, OCT_VEC2_NULL, 0.0f)) {
        float separation1 = OCT_vec2_dot(refNormal, sideClipped.start) - refFace.offset;
        if (separation1 > 0.0f) {
            contact1 = OCT_VEC2_NULL;
            valids = OCT_AorB_makeFalse(valids, OCT_A);
        }
    }
    if (!OCT_vec2_equal(sideClipped.end, OCT_VEC2_NULL, 0.0f)) {
        float separation2 = OCT_vec2_dot(refNormal, sideClipped.end) - refFace.offset;
        if (separation2 > 0.0f) {
            contact2 = OCT_VEC2_NULL;
            valids = OCT_AorB_makeFalse(valids, OCT_B);
        }
    }

    if (contactAOut) {
        *contactAOut = contact1;
    }
    if (contactBOut) {
        *contactBOut = contact2;
    }
    return valids;
}