#include "windowSystem_int.h"

#include "OCT_Math.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <stdbool.h>
#include <stdlib.h>

#include "window/window_int.h"
#include "inputs/inputs_int.h"

iOCT_windowSystem iOCT_windowSystem_inst = { 0 };

void OCT_WDWModule_init(char* name, unsigned int width, unsigned int height, OCT_vec4 color) {
	// iOCT_windowSystem_init(name, width, height, color);
}

void system_init_WINDOW() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow* initWindow = glfwCreateWindow(100, 100, "OCTAVIAN Init Window", NULL, NULL);
	glfwMakeContextCurrent(initWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	// GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
	// glfwMakeContextCurrent(window);
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

	// happens after registry init so using systemID is safe
	iOCT_windowSystem_inst.windowMap = eOCT_IDMap_init(iOCT_windowSystem_inst.windowSystem.systemID_reg, eOCT_POOL_CAPACITY_DEFAULT);
	iOCT_windowSystem_inst.windowPool = eOCT_pool_open(iOCT_windowSystem_inst.windowSystem.systemID_reg, eOCT_POOL_CAPACITY_DEFAULT, sizeof(iOCT_window));
	iOCT_windowSystem_inst.rootWindow = initWindow;
}


void eOCT_WINDOW_startFrame() {
	iOCT_window* window;
	eOCT_pool* windowPool = &iOCT_windowSystem_inst.windowPool;
	iOCT_window* windowArray = (iOCT_window*)windowPool->array;

	for (OCT_index windowCtr = 0; windowCtr < windowPool->count; windowCtr++) {
		window = &windowArray[windowCtr];
		glfwMakeContextCurrent(window->windowPtr);
		iOCT_window_poll(window);

		glClear(GL_COLOR_BUFFER_BIT);

		// check if windows should close
		if (glfwWindowShouldClose(window->windowPtr)) {
			iOCT_window_close(window, windowCtr);
			windowCtr--;
		}

	}

	// eOCT_pool* singlePool =
}

void eOCT_WINDOW_finishFrame() {
	eOCT_pool* windowPool = &iOCT_windowSystem_inst.windowPool;
	iOCT_window* windowArray = (iOCT_window*)windowPool->array;

	for (OCT_index windowCtr = 0; windowCtr < windowPool->count; windowCtr++) {
		iOCT_window window = windowArray[windowCtr];
		glfwMakeContextCurrent(window.windowPtr);

		glfwSwapBuffers(window.windowPtr);
	}
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