#pragma once
#include "inputs.h"

#include "OCT_Core_eng.h"

struct iOCT_buttonStates {
    OCT_BUTTONS button;
    OCT_buttonStates state;
    
};

void iOCT_input_keyCallback(OCT_index eventIndex);
void iOCT_input_mouseButtonCallback(OCT_index eventIndex);
void iOCT_input_mouseMoveCallback(OCT_index eventIndex);