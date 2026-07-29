
#include "cOCT_Communication.h"
#include "cOCT_EngineStructure.h"
#include "input/types.h"
#include <assert.h>
#include <stdio.h>

#include "input/keys/keys_internal.h"

void iOCT_INPModule_handleMSGs() {
	cOCT_message msg = cOCT_message_pop(OCT_subsystem_input);

	while (msg.messageType != cOCT_MSG_ALLCLEAR) {
		assert(msg.messageType == cOCT_MSG_KEY_DOWN || msg.messageType == cOCT_MSG_KEY_UP);
		if (msg.messageType == cOCT_MSG_KEY_DOWN) {
			iOCT_keyState_set(msg.key_event.key, OCT_KEYSTATE_DOWN);
		}
		else {
			iOCT_keyState_set(msg.key_event.key, OCT_KEYSTATE_UP);
			printf("Key: %d\n", msg.key_event.key);
		}

		msg = cOCT_message_pop(OCT_subsystem_input);
	}
}