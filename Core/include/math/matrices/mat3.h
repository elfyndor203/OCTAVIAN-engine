#pragma once

#include "math/vectors/vec2.h"
#include "math/vectors/vec3.h"

/// <summary>
/// Row-major 3x3 matrix
/// </summary>
typedef struct OCT_mat3 {
	float c0r0;
	float c0r1;
	float c0r2;

	float c1r0;
	float c1r1;
	float c1r2;

	float c2r0;
	float c2r1;
	float c2r2;
} OCT_mat3;

extern OCT_mat3 OCT_mat3_identity;

OCT_mat3 OCT_mat3_mul(OCT_mat3 parentMat, OCT_mat3 childMat);
OCT_vec3 OCT_mat3_mulVec3(OCT_mat3 matrix, OCT_vec3 vector);
OCT_mat3 OCT_mat3_generate(OCT_vec2 translation, OCT_vec2 scale, float rotation);
OCT_mat3 OCT_mat3_inverse(OCT_mat3 matrix);
OCT_mat3 OCT_mat3_translate(OCT_mat3 matrix, OCT_vec2 translation);
OCT_mat3 OCT_mat3_scale(OCT_mat3 matrix, OCT_vec2 scale);
OCT_mat3 OCT_mat3_rotate(OCT_mat3 matrix, float rotation);
void OCT_mat3_print(OCT_mat3 matrix);