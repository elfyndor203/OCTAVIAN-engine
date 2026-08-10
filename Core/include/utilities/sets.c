#include "sets.h"

#include <stdbool.h>

bool OCT_AorB_any(OCT_AorB value) {
    return value != OCT_NEITHER;
}

bool OCT_AorB_one(OCT_AorB value) {
    return value == OCT_A || value == OCT_B;
}

OCT_AorB OCT_AorB_NOT(OCT_AorB value) {
    return value ^ OCT_BOTH;
}

OCT_AorB OCT_AorB_makeFalse(OCT_AorB initial, OCT_AorB nowFalse) {
    return initial & ~nowFalse; // flips false from initial
}

OCT_AorB OCT_AorB_makeTrue(OCT_AorB initial, OCT_AorB nowTrue) {
    return initial | nowTrue;
}

bool OCT_AorB_isTrue(OCT_AorB value, OCT_AorB toCheck) {
    return value == toCheck || value == OCT_BOTH;
}

bool OCT_AorB_isFalse(OCT_AorB value, OCT_AorB toCheck) {
    return !(value == toCheck || value == OCT_BOTH);
}


