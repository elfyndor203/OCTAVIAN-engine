#include "line2.h"

OCT_vec2 OCT_line2_vec2(OCT_segment2 segment) {
    return OCT_vec2_sub(segment.end, segment.start);
}
// need to understand this
OCT_segment2 OCT_line2_clip(OCT_line2 line, OCT_segment2 segment) {
    float d1 = OCT_vec2_dot(line.normal, segment.start) - line.offset;
    float d2 = OCT_vec2_dot(line.normal, segment.end) - line.offset;

    if (d1 <= 0 && d2 <= 0) {
        return segment;
    }
    if (d1 > 0 && d2 > 0) {
        return OCT_SEGMENT_NULL;
    }

    float t = d1 / (d1 - d2);
    OCT_vec2 intersect = OCT_vec2_add(segment.start, OCT_vec2_mul(OCT_line2_vec2(segment), t));

    OCT_segment2 clipped;
    if (d1 > 0) {
        clipped.start = intersect;
        clipped.end = segment.end;
    }
    else {
        clipped.end = intersect;
        clipped.start = segment.start;
    }
    return clipped;
}