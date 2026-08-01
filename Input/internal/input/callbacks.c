
#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

void iOCT_input_keyCallback(OCT_index eventIndex) {
    eOCT_pool keyEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.keyTicket);
    eOCT_pool pressEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.keyPressTicket);
    eOCT_pool releaseEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.keyReleaseTicket);

    int key = *(int*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.keyTicket, eventIndex);
    bool pressed = *(bool*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.keyPressTicket, eventIndex);
    bool released = *(bool*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.keyReleaseTicket, eventIndex);

    if (pressed) {
        printf("Key %c pressed\n", key);
    }
    if (released) {
        printf("Key %c released\n", key);
    }
}

void iOCT_input_mouseButtonCallback(OCT_index eventIndex) {
    eOCT_pool keyEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.mouseButtonTicket);
    eOCT_pool pressEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.mouseButtonPressTicket);
    eOCT_pool releaseEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.mouseButtonReleaseTicket);

    int button = *(int*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.mouseButtonTicket, eventIndex);
    bool pressed = *(bool*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.mouseButtonPressTicket, eventIndex);
    bool released = *(bool*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.mouseButtonReleaseTicket, eventIndex);

    if (pressed) {
        printf("Mouse button %d pressed\n", button);
    }
    if (released) {
        printf("Mouse button %d released\n", button);
    }
}

void iOCT_input_mouseMoveCallback(OCT_index eventIndex) {
    eOCT_pool mouseXEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.mousePositionXTicket);
    eOCT_pool mouseYEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.mousePositionYTicket);

    float xPos = *(float*)eOCT_field_read(mouseXEventPool, iOCT_inputSystem_inst.mousePositionXTicket, eventIndex);
    float yPos = *(float*)eOCT_field_read(mouseYEventPool, iOCT_inputSystem_inst.mousePositionYTicket, eventIndex);

    iOCT_inputSystem_inst.mousePosition = (OCT_vec2){xPos, yPos};
}