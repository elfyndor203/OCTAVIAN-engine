#include "windowSystem_int.h"

#include "OCT_Math.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdbool.h>
#include <stdlib.h>

#include "window/window_int.h"
#include "inputs/inputs_internal.h"

iOCT_windowSystem iOCT_windowSystem_inst = { 0 };

void OCT_WDWModule_init(char* name, unsigned int width, unsigned int height, OCT_vec4 color) {
	// iOCT_windowSystem_init(name, width, height, color);
}

void iOCT_windowSystem_init() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
	// glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// glfwSetFramebufferSizeCallback(window, iOCT_window_callback_resize);
	// glfwSetKeyCallback(window, iOCT_window_callback_keyEvent);
	// glfwSetMouseButtonCallback(window, iOCT_window_callback_mouseEvent);
	// glfwSetCursorPosCallback(window, iOCT_window_callback_mouseMove);
	// glfwSetScrollCallback(window, iOCT_window_callback_mouseScroll);

	// glClearColor(color.x, color.y, color.z, color.a);

	// glfwSwapInterval(0);
	// iOCT_windowSystem_inst.windowPtr = window;
	// iOCT_windowSystem_inst.targetResolution = (OCT_vec2){ width, height };
	// iOCT_windowSystem_inst.currentResolution = iOCT_windowSystem_inst.targetResolution;
	// iOCT_window_viewport(width, height);
	// iOCT_keyMap_init();
	// iOCT_mouseMap_init();
	iOCT_windowSystem_inst.mainWindow = NULL;
}

void system_update_WINDOW() {
	iOCT_window* window;
	eOCT_pool* windowPool = eOCT_getDataPool_global(iOCT_windowSystem_inst.windowCache, NULL);
	iOCT_window* windowArray = (iOCT_window*)windowPool->array;

	for (OCT_index windowCtr = 0; windowCtr < windowPool->count; windowCtr++) {
		window = &windowArray[windowCtr];
		iOCT_window_poll(window);

		// check if windows should close
		if (glfwWindowShouldClose(window->windowPtr)) {
			window->open = false;
			glfwDestroyWindow(window->windowPtr);
			eOCT_pool_deleteEntry(windowPool, windowCtr, true); // remove the window
			windowCtr--;
		}
	}

	eOCT_pool* keyPool = eOCT_getDataPool_global(iOCT_windowSystem_inst.keyCache, NULL);
	eOCT_pool_clear(keyPool);
}

// void OCT_WDWModule_update() {
// 	iOCT_windowSystem_inst.cursorDelta = OCT_vec2_zero;
// 	iOCT_windowSystem_inst.scrollDelta = OCT_vec2_zero;
// 	iOCT_window_show();
// 	iOCT_window_wipe();
// }

// bool OCT_window_closed() {
// 	if (glfwWindowShouldClose(iOCT_windowSystem_inst.windowPtr)) {
// 		return true;
// 	}
// 	else {
// 		return false;
// 	}
// }

// void OCT_WDWModule_free() {
// 	free(iOCT_windowSystem_inst.keyMap);
// 	free(iOCT_windowSystem_inst.mouseMap);
// 	free(iOCT_windowSystem_inst.windowPtr);
// }