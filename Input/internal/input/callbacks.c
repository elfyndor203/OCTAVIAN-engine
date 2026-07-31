
#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

void iOCT_input_keyCallback(OCT_index eventIndex) {
    eOCT_pool keyEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.keyCache);
    eOCT_pool pressEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.keyPressCache);
    eOCT_pool releaseEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.keyReleaseCache);

    int key = *(int*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.keyCache, eventIndex);
    bool pressed = *(bool*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.keyPressCache, eventIndex);
    bool released = *(bool*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.keyReleaseCache, eventIndex);

    if (pressed) {
        printf("Key %c pressed\n", key);
    }
    if (released) {
        printf("Key %c released\n", key);
    }
}

void iOCT_input_mouseButtonCallback(OCT_index eventIndex) {
    eOCT_pool keyEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.mouseButtonCache);
    eOCT_pool pressEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.mouseButtonPressCache);
    eOCT_pool releaseEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.mouseButtonReleaseCache);

    int button = *(int*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.mouseButtonCache, eventIndex);
    bool pressed = *(bool*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.mouseButtonPressCache, eventIndex);
    bool released = *(bool*)eOCT_field_read(keyEventPool, iOCT_inputSystem_inst.mouseButtonReleaseCache, eventIndex);

    if (pressed) {
        printf("Mouse button %d pressed\n", button);
    }
    if (released) {
        printf("Mouse button %d released\n", button);
    }
}

void iOCT_input_mouseMoveCallback(OCT_index eventIndex) {
    eOCT_pool mouseXEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.mousePositionXCache);
    eOCT_pool mouseYEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_inputSystem_inst.mousePositionYCache);

    float xPos = *(float*)eOCT_field_read(mouseXEventPool, iOCT_inputSystem_inst.mousePositionXCache, eventIndex);
    float yPos = *(float*)eOCT_field_read(mouseYEventPool, iOCT_inputSystem_inst.mousePositionYCache, eventIndex);

    iOCT_inputSystem_inst.mousePosition = (OCT_vec2){xPos, yPos};
}