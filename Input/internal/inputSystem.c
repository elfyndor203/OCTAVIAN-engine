#include "inputSystem_int.h"

#include "input/inputs_int.h"

iOCT_inputSystem iOCT_inputSystem_inst = {0};

void system_init_INPUT() {
    eOCT_event_subscribe(iOCT_inputSystem_inst.keyTicket, OCT_HANDLE_NULL, iOCT_input_keyCallback);
    eOCT_event_subscribe(iOCT_inputSystem_inst.mouseButtonTicket, OCT_HANDLE_NULL, iOCT_input_mouseButtonCallback);
    eOCT_event_subscribe(iOCT_inputSystem_inst.mousePositionXTicket, OCT_HANDLE_NULL, iOCT_input_mouseMoveCallback); // __NOTE__ subscribing to two fields can result in double callbacks right now
}

void eOCT_INPUT_update() {
    double deltaTime = *(double*)eOCT_field_readOnce(iOCT_inputSystem_inst.deltaTimeTicket, OCT_INDEX_NULL, OCT_HANDLE_NULL);
    printf("Delta time: %f\n", deltaTime);
}