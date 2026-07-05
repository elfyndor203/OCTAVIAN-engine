#include "window_int.h"
#include "OCT_Core_eng.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

#include "windowSystem/windowSystem_int.h"

OCT_handle OCT_window_open(const char* name, unsigned int sizeX, unsigned int sizeY, OCT_vec4 color) {
    printf("\nOpening window %s\n", name);
    GLFWwindow* windowPtr = glfwCreateWindow(sizeX, sizeY, name, NULL, iOCT_windowSystem_inst.rootWindow);
    glfwMakeContextCurrent(windowPtr);

    int frameBufferX;
    int frameBufferY;
    glfwSwapInterval(1);
    glfwSetKeyCallback(windowPtr, iOCT_window_keyCallback);
    glfwGetFramebufferSize(windowPtr, &frameBufferX, &frameBufferY);
    glViewport(0, 0, frameBufferX, frameBufferY);
    glClearColor(1.0f, 0.5f, 0.0f, 1.0f); // __NOTE__ PASS AS PARAM

    GLuint newVAO;
    OCT_ID newID;
    iOCT_window* newWindow = eOCT_addGlobalDataEntry(iOCT_windowSystem_inst.windowCache, true, &newID);
    newWindow->windowID = newID;
    newWindow->windowPtr = windowPtr;
    newWindow->targetResolution = (OCT_vec2){ (float)sizeX, (float)sizeY };
    newWindow->currentResolution = (OCT_vec2){ (float)sizeX, (float)sizeY };
    glGenVertexArrays(1, &newVAO);
    newWindow->VAO = newVAO;
    OCT_handle windowHandle = {
        .containerID = OCT_ID_ECS,
        .objectID = newID,
    };

    printf("Created window %s %p\n", name, windowPtr);
    return windowHandle;
}

bool OCT_window_isOpen(OCT_handle windowHandle) {
    iOCT_window* window = (iOCT_window*)eOCT_getGlobalDataEntry(iOCT_windowSystem_inst.windowCache, windowHandle.objectID);
    if (!window) {
        return false;
    }
    return true;
}

bool OCT_window_anyOpen() {
    eOCT_pool* windowPool = (eOCT_pool*)eOCT_getDataPool_global(iOCT_windowSystem_inst.windowCache, NULL);
    return windowPool->count;
}

void iOCT_window_close(iOCT_window* window, OCT_index windowIndex) {
    eOCT_pool* windowPool = eOCT_getDataPool_global(iOCT_windowSystem_inst.windowCache, NULL);

    glfwDestroyWindow(window->windowPtr);
    eOCT_pool_deleteEntry(windowPool, windowIndex, true); // remove the window
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
