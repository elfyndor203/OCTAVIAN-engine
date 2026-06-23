#include "shapes/rect2.h"

#include "vectors/vec2.h"
#include "units/constants.h"
#include <math.h>

OCT_vec2 OCT_rect2_getVerts(OCT_rect2 rect, OCT_vec2* botR, OCT_vec2* topR, OCT_vec2* topL) {
    float halfW = rect.width / 2;
    float halfH = rect.height / 2;
    OCT_vec2 offsets[4] = {
        {-halfW, -halfH},   // BL
        { halfW, -halfH},   // BR
        { halfW,  halfH},   // TR
        {-halfW,  halfH},   // TL
    };
    OCT_vec2 botL = OCT_vec2_add(OCT_vec2_rotate(offsets[0], rect.rotation), rect.origin);
    if (botR) {
        *botR = OCT_vec2_add(OCT_vec2_rotate(offsets[1], rect.rotation), rect.origin);
    }
    if (topR) {
        *topR = OCT_vec2_add(OCT_vec2_rotate(offsets[2], rect.rotation), rect.origin);
    }
    if (topL) {
        *topL = OCT_vec2_add(OCT_vec2_rotate(offsets[3], rect.rotation), rect.origin);
    }
    return botL;
}

OCT_vec2 OCT_rect2_getNormals(OCT_rect2 rect, OCT_vec2* heightAxis) {
    OCT_vec2 widthAxis = OCT_vec2_norm(OCT_vec2_rotate((OCT_vec2) { rect.width, 0 }, rect.rotation));
    if (heightAxis) {
        *heightAxis = (OCT_vec2){ -widthAxis.y, widthAxis.x };
    }
    return widthAxis;
}