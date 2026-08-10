#pragma once

#include <stdbool.h>

typedef enum OCT_AorB {
    OCT_NEITHER = 0,    // bitflags, don't change
    OCT_A       = 1,
    OCT_B       = 2,
    OCT_BOTH    = 3
} OCT_AorB;

bool OCT_AorB_any(OCT_AorB value);
bool OCT_AorB_one(OCT_AorB value);
OCT_AorB OCT_AorB_NOT(OCT_AorB value);
OCT_AorB OCT_AorB_makeFalse(OCT_AorB initial, OCT_AorB nowFalse);
OCT_AorB OCT_AorB_makeTrue(OCT_AorB initial, OCT_AorB nowTrue);
bool OCT_AorB_isTrue(OCT_AorB value, OCT_AorB toCheck);
bool OCT_AorB_isFalse(OCT_AorB value, OCT_AorB toCheck);