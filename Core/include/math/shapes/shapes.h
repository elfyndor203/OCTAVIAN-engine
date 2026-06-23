#pragma once
#include "rect2.h"

#include "vectors/vec2.h"

typedef enum {
	OCT_shapeType_rect2,
	OCT_shapeType_caps2,
	OCT_shapeType_circ2,
} OCT_shapeType;

typedef struct {
	OCT_vec2 origin;
	float radius;
} OCT_circ2;

typedef struct {
	OCT_vec2 origin;
	float radius;
	float aLen;
} OCT_caps2;

typedef struct {
	OCT_shapeType type;
	union {
		OCT_rect2 rect2;
		OCT_circ2 circ2;
		OCT_caps2 caps2;
	};
} OCT_shape2;
