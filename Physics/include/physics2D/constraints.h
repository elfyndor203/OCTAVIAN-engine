#pragma once
#include "OCT_Core.h"

OCT_local OCT_rope2D_new(OCT_local entityA, OCT_local entityB, float length);
void OCT_rope2D_length(OCT_local rope2D, float newLength);
bool OCT_rope2D_disable(OCT_local rope2D);
bool OCT_rope2D_enable(OCT_local rope2D);

OCT_local OCT_hitbox2D_new(OCT_local entity, OCT_vec2 dimensions, OCT_vec2 position, float rotation);