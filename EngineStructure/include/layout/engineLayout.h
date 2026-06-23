#pragma once
#include "types.h"

typedef enum {
	OCT_subsystem_ECS,
	OCT_subsystem_renderer,
	OCT_subsystem_resources,
	OCT_subsystem_window,
	OCT_subsystem_input,
	OCT_subsystem_physics,

	OCT_subsystem_count
} OCT_subsystemList;

typedef enum {
	OCT_handle_NULL = 0,

	OCT_handle_entityContext,
	OCT_handle_entity,
	OCT_handle_transform2D,
	OCT_handle_sprite2D,
	OCT_handle_collider2D,
	OCT_handle_camera2D,

	OCT_handle_layer,
	
	OCT_handle_image,
	OCT_handle_mesh,

	OCT_handle_window
} OCT_handleType;

struct OCT_handle {
	OCT_ID system;
	OCT_ID containerID;
	OCT_ID objectID;
	OCT_handleType type;
};

