#include "systemEx_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>

iOCT_systemEx iOCT_systemEx_inst = { 0 };

static void iOCT_input_keyCallback(OCT_index eventIndex);

void system_init_systemEx() {
    eOCT_event_subscribe(iOCT_systemEx_inst.keyCache, OCT_HANDLE_NULL, iOCT_input_keyCallback);
}

static void iOCT_input_keyCallback(OCT_index eventIndex) {
    int key = *(int*)eOCT_event_read(iOCT_systemEx_inst.keyCache, OCT_HANDLE_NULL, eventIndex);
    bool pressed = *(bool*)eOCT_event_read(iOCT_systemEx_inst.keyPressCache, OCT_HANDLE_NULL, eventIndex);
    bool released = *(bool*)eOCT_event_read(iOCT_systemEx_inst.keyReleaseCache, OCT_HANDLE_NULL, eventIndex);

    if (pressed) {
        printf("Key %c pressed\n", key);
    }
    if (released) {
        printf("Key %c released\n", key);
    }
}