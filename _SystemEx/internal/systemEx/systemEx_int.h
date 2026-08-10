#pragma once
#include "OCT_Core_eng.h"
#include "types_int.h"

struct iOCT_systemEx {
	OCT_ID systemID;
	eOCT_fieldTicket keyCache;
	eOCT_fieldTicket keyPressCache;
	eOCT_fieldTicket keyReleaseCache;
};

extern iOCT_systemEx iOCT_systemEx_inst;

void system_init_systemEx();