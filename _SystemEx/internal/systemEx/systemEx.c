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
    eOCT_pool keyEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_systemEx_inst.keyCache);
    eOCT_pool pressEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_systemEx_inst.keyPressCache);
    eOCT_pool releaseEventPool = *eOCT_field_getSourcePool(OCT_HANDLE_NULL, iOCT_systemEx_inst.keyReleaseCache);

    int key = *(int*)eOCT_field_read(keyEventPool, iOCT_systemEx_inst.keyCache, eventIndex);
    bool pressed = *(bool*)eOCT_field_read(keyEventPool, iOCT_systemEx_inst.keyPressCache, eventIndex);
    bool released = *(bool*)eOCT_field_read(keyEventPool, iOCT_systemEx_inst.keyReleaseCache, eventIndex);

    if (pressed) {
        printf("Key %c pressed\n", key);
    }
    if (released) {
        printf("Key %c released\n", key);
    }
}