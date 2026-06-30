#include "window_int.h"
#include "OCT_Core_eng.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

#include "windowSystem/windowSystem_int.h"

OCT_handle OCT_window_open(const char* name, unsigned int sizeX, unsigned int sizeY, OCT_vec4 color) {
    iOCT_window window;

    printf("\nOpening window '%s'\n %2cType: ", name, ' ');
    GLFWwindow* windowPtr;
    if (iOCT_windowSystem_inst.mainWindow == NULL) {
        printf("Main / Resource\n");
        windowPtr = glfwCreateWindow(sizeX, sizeY, name, NULL, NULL);
        iOCT_windowSystem_inst.mainWindow = windowPtr;

        glfwMakeContextCurrent(windowPtr);
    } else {
        printf("Auxiliary\n");
        windowPtr = glfwCreateWindow(sizeX, sizeY, name, NULL, iOCT_windowSystem_inst.mainWindow);
    }

    glfwMakeContextCurrent(windowPtr);
    glfwSwapInterval(1);

    glfwSetKeyCallback(windowPtr, iOCT_window_keyCallback);

    window.windowPtr = windowPtr;
    window.targetResolution = (OCT_vec2){ (float)sizeX, (float)sizeY };
    window.currentResolution = (OCT_vec2){ (float)sizeX, (float)sizeY };
    window.open = true;

    OCT_ID windowID;
    iOCT_window* windowDestination = eOCT_addGlobalDataEntry(iOCT_windowSystem_inst.windowCache, true, &windowID);
    *windowDestination = window;

    OCT_handle windowHandle = {
        .containerID = OCT_ID_ECS,
        .objectID = windowID,
        .system = iOCT_windowSystem_inst.windowSystem.systemID_reg,
    };

    return windowHandle;
}

bool OCT_window_isOpen(OCT_handle windowHandle) {
    iOCT_window* window = (iOCT_window*)eOCT_getGlobalDataEntry(iOCT_windowSystem_inst.windowCache, windowHandle.objectID);
    if (!window) {
        return false;
    }
    return window->open;
}

void iOCT_window_poll(iOCT_window* window) {
    window->cursorDelta = OCT_vec2_zero;
    glfwPollEvents();
}
// void OCT_window_wipe() {
// 	iOCT_window_wipe();
// }
// void iOCT_window_wipe() {
// 	glClear(GL_COLOR_BUFFER_BIT);
// }
//
// void OCT_window_show() {
// 	iOCT_window_show();
// }
// void iOCT_window_show() {
// 	GLFWwindow* window = iOCT_windowSystem_inst.windowPtr;
// 	glfwSwapBuffers(window);
// }
//
// void iOCT_window_viewport(int width, int height) {
// 	glViewport(0, 0, width, height);
//
//     float aspect = width / height;
// 	float targetAspect = iOCT_windowSystem_inst.targetResolution.x / iOCT_windowSystem_inst.targetResolution.y;
//
//     int viewWidth, viewHeight, offsetX, offsetY;
//
//     if (aspect > targetAspect) { // too wide
//         viewHeight = height;
//         viewWidth = (int)(height * targetAspect);
//         offsetX = (width - viewWidth) / 2;
//         offsetY = 0;
//
//     }
//     else { // too tall
//         viewWidth = width;
//         viewHeight = (int)(width / targetAspect);
//         offsetX = 0;
//         offsetY = (height - viewHeight) / 2;
//     }
//     iOCT_windowSystem_inst.windowOffset.x = offsetX;
//     iOCT_windowSystem_inst.windowOffset.y = offsetY;
//
//     glViewport(offsetX, offsetY, viewWidth, viewHeight);
// }
//
// #pragma region callbacks
// void iOCT_window_callback_resize(GLFWwindow* window, int newWidth, int newHeight) {
//     iOCT_windowSystem_inst.currentResolution = (OCT_vec2){ newWidth, newHeight };
//     iOCT_window_viewport(newWidth, newHeight);
// }
// #pragma endregion
//
// #pragma region cross-module requests
// OCT_vec2 _OCT_window_getResolution(OCT_vec2* outViewPortOffset) {
//     *outViewPortOffset = iOCT_windowSystem_inst.windowOffset;
//     return iOCT_windowSystem_inst.currentResolution;
// }
// #pragma endregion
