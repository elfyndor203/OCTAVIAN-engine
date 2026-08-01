#pragma once

#include "OCT_Core.h"

typedef enum OCT_keyStates {
    OCT_KEYSTATE_NONE,
    OCT_KEYSTATE_RELEASED,
    OCT_KEYSTATE_PRESSED,
    OCT_KEYSTATE_HELD
} OCT_keyStates;

OCT_vec2 OCT_mouse_readPosWindow();