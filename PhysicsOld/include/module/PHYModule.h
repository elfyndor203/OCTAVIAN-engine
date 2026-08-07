#pragma once
#include "OCT_Math.h"

#define OCT_GRAVITY_DEFAULT -80

void OCT_PHYModule_init(OCT_vec2 gravity, unsigned int hz);
void OCT_PHYModule_update();