#include "INPModule_internal.h"
#include "types_internal.h"

iOCT_INPModule iOCT_INPModule_instance = { 0 };

void OCT_INPModule_update() {
	iOCT_keyState_convert();
	iOCT_INPModule_handleMSGs();
}