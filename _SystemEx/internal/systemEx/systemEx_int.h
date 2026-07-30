#pragma once
#include "OCT_Core_eng.h"
#include "types_int.h"

struct iOCT_systemEx {
	eOCT_systemDescription system; // stable systemDescription storage
	eOCT_fieldRequest keyCache;
	eOCT_fieldRequest keyPressCache;
	eOCT_fieldRequest keyReleaseCache;
};

extern iOCT_systemEx iOCT_systemEx_inst;

void system_init_systemEx();