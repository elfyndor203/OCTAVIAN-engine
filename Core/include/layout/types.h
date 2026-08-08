#pragma once

#include "OCT_Math.h"
#include <stdint.h>
#include <stdbool.h>

#define OCT_ID_NULL 0
#define OCT_GLOBAL_NULL ((OCT_global) {OCT_ID_NULL, OCT_ID_NULL})
#define OCT_LOCAL_NULL ((OCT_local) {OCT_GLOBAL_NULL, OCT_ID_NULL, OCT_ID_NULL})

typedef uint64_t OCT_ID;

typedef struct OCT_global {
	OCT_ID systemID;
	OCT_ID containerID;
	OCT_ID objectID;
} OCT_global;

typedef struct OCT_local {
	OCT_global contextHandle;
	OCT_ID containerID;
	OCT_ID objectID;

	//OCT_index generation;
} OCT_local;

bool OCT_local_isNULL(OCT_local handle);
bool OCT_global_isNULL(OCT_global handle);
bool OCT_handle_isEqual(OCT_local handle1, OCT_local handle2);

