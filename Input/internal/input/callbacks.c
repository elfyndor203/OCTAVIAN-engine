
#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

void iOCT_input_keyCallback(OCT_index eventIndex) {
    int key = *(int*)eOCT_event_read(iOCT_inputSystem_inst.keyCache, OCT_HANDLE_NULL, eventIndex);
    bool pressed = *(bool*)eOCT_event_read(iOCT_inputSystem_inst.keyPressCache, OCT_HANDLE_NULL, eventIndex);
    bool released = *(bool*)eOCT_event_read(iOCT_inputSystem_inst.keyReleaseCache, OCT_HANDLE_NULL, eventIndex);

    if (pressed) {
        printf("Key %c pressed\n", key);
    }
    if (released) {
        printf("Key %c released\n", key);
    }
}

void iOCT_input_mouseButtonCallback(OCT_index eventIndex) {
    int button = *(int*)eOCT_event_read(iOCT_inputSystem_inst.mouseButtonCache, OCT_HANDLE_NULL, eventIndex);
    bool pressed = *(bool*)eOCT_event_read(iOCT_inputSystem_inst.mouseButtonPressCache, OCT_HANDLE_NULL, eventIndex);
    bool released = *(bool*)eOCT_event_read(iOCT_inputSystem_inst.mouseButtonReleaseCache, OCT_HANDLE_NULL, eventIndex);

    if (pressed) {
        printf("Mouse button %d pressed\n", button);
    }
    if (released) {
        printf("Mouse button %d released\n", button);
    }
}

void iOCT_input_mouseMoveCallback(OCT_index eventIndex) {
    float xPos = *(float*)eOCT_event_read(iOCT_inputSystem_inst.mousePositionXCache, OCT_HANDLE_NULL, eventIndex);
    float yPos = *(float*)eOCT_event_read(iOCT_inputSystem_inst.mousePositionYCache, OCT_HANDLE_NULL, eventIndex);

    iOCT_inputSystem_inst.mousePosition = (OCT_vec2){xPos, yPos};
}