#pragma once

#include "OCT_Math.h"
#include <stdbool.h>

OCT_vec2 OCT_cursorPos_read(bool virtual);
OCT_vec2 OCT_cursorDelta_read(bool virtual);
OCT_vec2 OCT_scrollDelta_read();