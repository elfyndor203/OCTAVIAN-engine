#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

struct iOCT_transform2D {
	OCT_ID entityID;

	OCT_vec2 position;
	float rotation;
	OCT_vec2 scale;

	OCT_ID parentEntityID;
	OCT_index depth;

	OCT_mat3 localMatrix;
	OCT_mat3 globalMatrix;
};

void iOCT_transform2D_generateRoot(OCT_handle rootEntity);

