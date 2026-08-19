#pragma once

#include "OCT_Core.h"

typedef enum OCT_buttonStates {
    OCT_BUTTONSTATE_NONE,
    OCT_BUTTONSTATE_RELEASED,
    OCT_BUTTONSTATE_PRESSED,
    OCT_BUTTONSTATE_HELD
} OCT_buttonStates;
//
// OCT_vec2 OCT_mouse_readPosWindow();
// OCT_vec2 OCT_mouse_readPosWorld();
float OCT_mouse_readScroll();

OCT_buttonStates OCT_button_read(OCT_BUTTON button, double* timeSinceLastStateChange_out, OCT_index* framesSinceLastStateChange_out);
