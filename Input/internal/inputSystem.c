#include "inputSystem_int.h"

#include "input/inputs_int.h"

iOCT_inputSystem iOCT_inputSystem_inst = {0};

void system_init_INPUT() {
    eOCT_event_subscribe(iOCT_inputSystem_inst.keyCache, OCT_HANDLE_NULL, iOCT_input_keyCallback);
    eOCT_event_subscribe(iOCT_inputSystem_inst.mouseButtonCache, OCT_HANDLE_NULL, iOCT_input_mouseButtonCallback);
    eOCT_event_subscribe(iOCT_inputSystem_inst.mousePositionXCache, OCT_HANDLE_NULL, iOCT_input_mouseMoveCallback); // __NOTE__ subscribing to two fields can result in double callbacks right now
}