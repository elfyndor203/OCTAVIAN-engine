#include "math/matrices/mat3.h"
#include "math/vectors/vec2.h"
#include "math/vectors/vec3.h"

#include <math.h>
#include <assert.h>
#include <stdio.h>

const OCT_mat3 OCT_mat3_identity = {1, 0, 0, 0, 1, 0, 0, 0, 1};

OCT_mat3 OCT_mat3_mul(OCT_mat3 matA, OCT_mat3 matB) {
    OCT_mat3 result = {
        matA.c0r0 * matB.c0r0 + matA.c1r0 * matB.c0r1 + matA.c2r0 * matB.c0r2,  // c0r0
        matA.c0r1 * matB.c0r0 + matA.c1r1 * matB.c0r1 + matA.c2r1 * matB.c0r2,  // c0r1
        matA.c0r2 * matB.c0r0 + matA.c1r2 * matB.c0r1 + matA.c2r2 * matB.c0r2,  // c0r2
        matA.c0r0 * matB.c1r0 + matA.c1r0 * matB.c1r1 + matA.c2r0 * matB.c1r2,  // c1r0
        matA.c0r1 * matB.c1r0 + matA.c1r1 * matB.c1r1 + matA.c2r1 * matB.c1r2,  // c1r1
        matA.c0r2 * matB.c1r0 + matA.c1r2 * matB.c1r1 + matA.c2r2 * matB.c1r2,  // c1r2
        matA.c0r0 * matB.c2r0 + matA.c1r0 * matB.c2r1 + matA.c2r0 * matB.c2r2,  // c2r0
        matA.c0r1 * matB.c2r0 + matA.c1r1 * matB.c2r1 + matA.c2r1 * matB.c2r2,  // c2r1
        matA.c0r2 * matB.c2r0 + matA.c1r2 * matB.c2r1 + matA.c2r2 * matB.c2r2,  // c2r2
    };
    return result;
}

OCT_vec3 OCT_mat3_mulVec3(OCT_mat3 mat, OCT_vec3 vec) {
    OCT_vec3 result = {
        mat.c0r0 * vec.x + mat.c1r0 * vec.y + mat.c2r0 * vec.z,
        mat.c0r1 * vec.x + mat.c1r1 * vec.y + mat.c2r1 * vec.z,
        mat.c0r2 * vec.x + mat.c1r2 * vec.y + mat.c2r2 * vec.z
    };

    return result;
}

OCT_mat3 OCT_mat3_generate(OCT_vec2 translation, OCT_vec2 scale, float rotation) {
    float cosRot = cosf(rotation);
    float sinRot = sinf(rotation);
    OCT_mat3 result = {
        scale.x * cosRot,  scale.x * sinRot, 0,   // column 0
       -scale.y * sinRot,  scale.y * cosRot, 0,   // column 1
        translation.x,     translation.y,    1     // column 2
    };
    return result;
}

// Claude generated
OCT_mat3 OCT_mat3_inverse(OCT_mat3 m) {
    // Scale squared from each column
    float sx2 = m.c0r0 * m.c0r0 + m.c0r1 * m.c0r1;
    float sy2 = m.c1r0 * m.c1r0 + m.c1r1 * m.c1r1;

    float inv_sx2 = 1.0f / sx2;
    float inv_sy2 = 1.0f / sy2;

    // Transposed 2x2 with inverse scale applied
    float r00 = m.c0r0 * inv_sx2;
    float r01 = m.c1r0 * inv_sy2;   // fixed
    float r10 = m.c0r1 * inv_sx2;   // fixed
    float r11 = m.c1r1 * inv_sy2;

    // Inverse translation
    float inv_tx = -(r00 * m.c2r0 + r10 * m.c2r1);
    float inv_ty = -(r01 * m.c2r0 + r11 * m.c2r1);

    OCT_mat3 result = {
        r00,  r01,  0,
        r10,  r11,  0,
        inv_tx, inv_ty, 1
    };
    return result;
}

OCT_mat3 OCT_mat3_translate(OCT_mat3 m, OCT_vec2 translation) {
    OCT_mat3 result = m;
    result.c2r0 += translation.x;
    result.c2r1 += translation.y;
    return result;
}

OCT_mat3 OCT_mat3_translateTo(OCT_mat3 m, OCT_vec2 translation) {
    OCT_mat3 result = m;
    result.c2r0 = translation.x;
    result.c2r1 = translation.y;
    return result;
}

OCT_mat3 OCT_mat3_scale(OCT_mat3 m, OCT_vec2 scale) {
    OCT_mat3 result = m;
    result.c0r0 *= scale.x;
    result.c1r0 *= scale.x;
    result.c2r0 *= scale.x;

    result.c0r1 *= scale.y;
    result.c1r1 *= scale.y;
    result.c2r1 *= scale.y;
    return result;
}

OCT_mat3 OCT_mat3_rotateTo(OCT_mat3 m, float rotation) {
    float cosRot = cosf(rotation);
    float sinRot = sinf(rotation);
    OCT_mat3 result = m;
    result.c0r0 = cosRot;
    result.c0r1 = sinRot;
    result.c1r0 = -sinRot;
    result.c1r1 = cosRot;
    return result;
}

// claude generated
OCT_mat3 OCT_mat3_rotate(OCT_mat3 m, float rotation) {
    float cosRot = cosf(rotation);
    float sinRot = sinf(rotation);
    OCT_mat3 result = m;

    float oldC0R0 = m.c0r0, oldC0R1 = m.c0r1;
    float oldC1R0 = m.c1r0, oldC1R1 = m.c1r1;

    result.c0r0 = cosRot * oldC0R0 - sinRot * oldC0R1;
    result.c0r1 = sinRot * oldC0R0 + cosRot * oldC0R1;

    result.c1r0 = cosRot * oldC1R0 - sinRot * oldC1R1;
    result.c1r1 = sinRot * oldC1R0 + cosRot * oldC1R1;

    return result;
}
// claude generated
float OCT_mat3_getRotation(OCT_mat3 m) {
    float len = sqrtf(m.c0r0 * m.c0r0 + m.c0r1 * m.c0r1);
    return atan2f(m.c0r1 / len, m.c0r0 / len);
}



OCT_vec2 OCT_mat3_getScale(OCT_mat3 m) {
    return (OCT_vec2){m.c0r0, m.c1r1};
}
OCT_vec2 OCT_mat3_getTranslation(OCT_mat3 m) {
    return (OCT_vec2){m.c2r0, m.c2r1};
}

void OCT_mat3_print(OCT_mat3 m) {
    printf("[%.2f, %.2f, %.2f]\n", m.c0r0, m.c1r0, m.c2r0);
    printf("[%.2f, %.2f, %.2f]\n", m.c0r1, m.c1r1, m.c2r1);
    printf("[%.2f, %.2f, %.2f]\n", m.c0r2, m.c1r2, m.c2r2);
}