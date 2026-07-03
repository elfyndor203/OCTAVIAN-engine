#pragma once

#include <stdint.h>
#include <stdbool.h>

#define OCT_ID_NULL 0

typedef uint64_t OCT_ID;

typedef struct OCT_handle {
	OCT_ID system;
	OCT_ID containerID;
	OCT_ID objectID;
	OCT_ID handleType;
	bool valid;
} OCT_handle;

extern OCT_handle OCT_HANDLE_NULL;

