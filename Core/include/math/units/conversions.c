#include "math/units/constants.h"
#include <math.h>

float OCT_deg2rad(float degrees) {
	return degrees * ((float)M_PI / 180.0f);
}

float OCT_rad2deg(float radians) {
	return radians * (180.0f / (float)M_PI);
}