#pragma once

#include "OCT_Math.h"
#include <stdint.h>
#include <stdbool.h>

#define OCT_ID_NULL 0

typedef uint64_t OCT_ID;

typedef struct OCT_handle {
	OCT_ID containerID;
	OCT_ID objectID;
	OCT_ID handleType;

	//OCT_index generation;
} OCT_handle;

extern OCT_handle OCT_HANDLE_NULL;

bool OCT_handle_isNULL(OCT_handle handle);
bool OCT_handle_isEqual(OCT_handle handle1, OCT_handle handle2);

