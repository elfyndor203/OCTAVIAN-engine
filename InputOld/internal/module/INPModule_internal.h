#pragma once
#include "input/keys/keys_internal.h"
#include "types_internal.h"

struct iOCT_INPModule {
	OCT_keyState keyStateList[OCT_KEYS_TOTAL];
};

extern iOCT_INPModule iOCT_INPModule_instance;

void iOCT_INPModule_handleMSGs();
