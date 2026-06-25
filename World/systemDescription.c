#include "OCT_Core_eng.h"
#include <stddef.h>

#include "position2D/position2D_int.h"

void system_register_WORLD() {
	eOCT_fieldDescription posX = { "pos.x", eOCT_FIELDTYPE_FLOAT32, offsetof(iOCT_position2D, x) };
}