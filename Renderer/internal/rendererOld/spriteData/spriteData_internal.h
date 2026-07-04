#pragma once
#include "OCT_Core_eng.h"

struct iOCT_sprite2D {
	OCT_mat3 transform;
	OCT_vec4 color;
	OCT_vec4 uvRect;
	OCT_vec2 dimensions;
};