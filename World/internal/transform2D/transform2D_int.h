#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"

#define iOCT_TRANSFORM_ROOT_DEPTH 0

struct iOCT_transform2D {
	OCT_handle entityHandle;

	OCT_vec2 position;
	float rotation;
	OCT_vec2 scale;

	OCT_handle parentEntityHandle;
	OCT_index depth;

	OCT_mat3 localMatrix;
	OCT_mat3 globalMatrix;
};

void iOCT_transform2D_generateRoot(OCT_handle rootEntity);
void iOCT_transform2D_propagate(OCT_handle context);

