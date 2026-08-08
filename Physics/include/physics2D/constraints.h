#pragma once
#include "OCT_Core.h"

OCT_local OCT_rope2D_constrain(OCT_local entityA, OCT_local entityB, float length);
void OCT_rope2D_length(OCT_local rope2D, float newLength);
bool OCT_rope2D_disable(OCT_local rope2D);
bool OCT_rope2D_enable(OCT_local rope2D);