#include "keys_internal.h"
#include "types_internal.h"

#include "cOCT_EngineStructure.h"
#include "input/types.h"
#include "module/INPModule_internal.h"

OCT_keyState OCT_keyState_read(OCT_key key) {
	return iOCT_keyState_read(key);
}
OCT_keyState iOCT_keyState_read(OCT_key key) {
	return iOCT_INPModule_instance.keyStateList[key];
}

void iOCT_keyState_set(OCT_key key, OCT_keyState state) {
	iOCT_INPModule_instance.keyStateList[key] = state;
}

void iOCT_keyState_convert() {	// update before getting new inputs
	OCT_keyState* keyState;
	for (int i = 0; i < OCT_KEYS_TOTAL; i++) {
		keyState = &iOCT_INPModule_instance.keyStateList[i];

		if (*keyState == OCT_KEYSTATE_DOWN) {
			*keyState = OCT_KEYSTATE_HELD;
		}
		if (*keyState == OCT_KEYSTATE_UP) {
			*keyState = OCT_KEYSTATE_IDLE;
		}
	}
}