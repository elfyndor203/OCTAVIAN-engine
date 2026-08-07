#pragma once
#include <stdbool.h>
#include "math/definitions/operations.h"

typedef struct {
	float x;
	float y;
} OCT_vec2;

extern const OCT_vec2 OCT_vec2_zero;

float OCT_vec2_dot(OCT_vec2 vectorA, OCT_vec2 vectorB);
float OCT_vec2_cross(OCT_vec2 vectorA, OCT_vec2 vectorB);
OCT_vec2 OCT_vec2_add(OCT_vec2 vectorA, OCT_vec2 vectorB);
/// <summary>
/// Element-wise subtraction of B from A
/// </summary>
/// <param name="vectorA"></param>
/// <param name="vectorB"></param>
/// <returns></returns>
OCT_vec2 OCT_vec2_sub(OCT_vec2 vectorA, OCT_vec2 vectorB);
OCT_vec2 OCT_vec2_mul_eleWise(OCT_vec2 vectorA, OCT_vec2 vectorB);
OCT_vec2 OCT_vec2_div_eleWise(OCT_vec2 vectorA, OCT_vec2 vectorB);

OCT_vec2 OCT_vec2_mul(OCT_vec2 vectorA, float scalar);
OCT_vec2 OCT_vec2_div(OCT_vec2 vectorA, float scalar);
OCT_vec2 OCT_vec2_rotate(OCT_vec2 vectorA, float degrees);

float OCT_vec2_mag(OCT_vec2 vectorA);
bool OCT_vec2_equal(OCT_vec2 vectorA, OCT_vec2 vectorB, float threshold);
float OCT_vec2_radians(OCT_vec2 vectorA, OCT_vec2 vectorB);
float OCT_vec2_degrees(OCT_vec2 vectorA, OCT_vec2 vectorB);

OCT_vec2 OCT_vec2_unit(OCT_vec2 vectorA);
OCT_vec2 OCT_vec2_neg(OCT_vec2 vectorA);

