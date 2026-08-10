#pragma once
#include "math/vectors/vec2.h"

#define OCT_SEGMENT_ZERO ((OCT_segment2){OCT_VEC2_ZERO, OCT_VEC2_ZERO})
#define OCT_SEGMENT_NULL ((OCT_segment2){OCT_VEC2_NULL, OCT_VEC2_NULL})

typedef struct {
    OCT_vec2 start;
    OCT_vec2 end;
} OCT_segment2;

typedef struct {
    OCT_vec2 normal;
    float offset;
} OCT_line2;

OCT_vec2 OCT_line2_vec2(OCT_segment2 segment);
OCT_segment2 OCT_line2_clip(OCT_line2 line, OCT_segment2 segment);