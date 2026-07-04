#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GL_CHECK_WIN() { GLenum err = glGetError(); if (err != GL_NO_ERROR) printf("GL error %d at line %d\n", err, __LINE__); }

struct iOCT_windowSystem {
	eOCT_systemDescription windowSystem;
	eOCT_dataPoolDescription windowCache;
	eOCT_dataPoolDescription keyCache;

	GLFWwindow* rootWindow; // holds gpu resources

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
