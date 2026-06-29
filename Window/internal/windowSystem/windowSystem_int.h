#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct iOCT_windowSystem {
	eOCT_systemDescription windowSystem;
	eOCT_dataPoolDescription windowCache;

	GLFWwindow* mainWindow; // holds gpu resources

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

void iOCT_windowSystem_init();
