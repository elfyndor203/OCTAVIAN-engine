#pragma once
#include "inputs.h"
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_buttonState {
    OCT_BUTTON button;
    OCT_buttonStates state;
    double timeSinceLastStateChange;
    OCT_index framesSinceLastStateChange;
    bool updatedThisFrame;
};

void iOCT_input_keyCallback(OCT_index eventIndex);
void iOCT_input_mouseButtonCallback(OCT_index eventIndex);
void iOCT_input_mouseMoveCallback(OCT_index eventIndex);
void iOCT_input_mouseScrollCallback(OCT_index eventIndex);

void iOCT_button_updateState(OCT_BUTTON button, OCT_buttonStates pressOrRelease);