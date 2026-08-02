#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct iOCT_windowSystem {
	eOCT_systemDescription windowSystem;
	eOCT_eventKey keyEventKey;
	eOCT_eventKey mouseButtonEventKey;
	eOCT_eventKey mouseMoveEventKey;
	eOCT_singleKey focusedCameraMatrixKey;

	eOCT_IDMap windowMap;
	eOCT_pool windowPool;

	GLFWwindow* rootWindow; // holds gpu resources
	OCT_ID focusedWindowID;
	// OCT_vec2 targetResolution;
	// OCT_vec2 currentResolution;
	// OCT_vec2 windowOffset;

	// OCT_key* keyMap;
	// OCT_key* mouseMap;
	// OCT_vec2 cursorPos;
	// OCT_vec2 cursorDelta;
	// OCT_vec2 scrollDelta;

};

extern iOCT_windowSystem iOCT_windowSystem_inst;

void system_init_WINDOW();
