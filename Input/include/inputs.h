#pragma once

#include "OCT_Core.h"

typedef enum OCT_buttonStates {
    OCT_BUTTONSTATE_NONE,
    OCT_BUTTONSTATE_RELEASED,
    OCT_BUTTONSTATE_PRESSED,
    OCT_BUTTONSTATE_HELD
} OCT_buttonStates;

OCT_vec2 OCT_mouse_readPosWindow();
OCT_vec2 OCT_mouse_readPosWorld();