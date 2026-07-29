#include "cursor_internal.h"

#include "OCT_Math.h"
#include <stdbool.h>
#include <stdio.h>

#include "_WDW_Output/_WDW_include.h"
#include "_REN_Output/_REN_include.h"

OCT_vec2 OCT_cursorPos_read(bool virtual) {
	OCT_vec2 pos = _OCT_window_cursorPos_get();
	if (virtual) {
		pos = _OCT_renderer_projectCoords(pos);
	}
	//printf("Coords: %f, %f\n", pos.x, pos.y);
	return pos;
}
OCT_vec2 OCT_cursorDelta_read(bool virtual) {
	OCT_vec2 dummy;
	OCT_vec2 delta = _OCT_window_cursorDelta_get();
	OCT_vec2 resolution = _OCT_window_getResolution(&dummy);
	OCT_vec2 virtualSize = _OCT_renderer_getVirtual();

	if (virtual) {
		delta.x *= virtualSize.x / resolution.x;
		delta.y *= virtualSize.y / resolution.y;
	}

	return delta;
}

OCT_vec2 OCT_scrollDelta_read() {
	return _OCT_window_scrollDelta_get();
}