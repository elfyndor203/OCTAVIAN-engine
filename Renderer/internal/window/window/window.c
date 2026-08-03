#include "window_int.h"
#include "window/types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>

#include "window/windowSystem_int.h"
#include "renderer/renderer_int.h"

OCT_handle OCT_window_open(const char* name, unsigned int sizeX, unsigned int sizeY, OCT_vec4 color) {
    printf("\nOpening window %s\n", name);
    GLFWwindow* windowPtr = glfwCreateWindow(sizeX, sizeY, name, NULL, iOCT_windowSystem_inst.rootWindow);
    glfwMakeContextCurrent(windowPtr);

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    // set initial framebuffer size
    int frameBufferX;
    int frameBufferY;
    glfwSwapInterval(1);

    glfwSetKeyCallback(windowPtr, iOCT_window_keyCallback);
    glfwSetMouseButtonCallback(windowPtr, iOCT_window_mouseButtonCallback);
    glfwSetCursorPosCallback(windowPtr, iOCT_window_mouseMoveCallback);
    glfwSetWindowFocusCallback(windowPtr, iOCT_window_focusCallback);

    glfwGetFramebufferSize(windowPtr, &frameBufferX, &frameBufferY);
    glViewport(0, 0, frameBufferX, frameBufferY);
    glClearColor(color.x, color.y, color.z, color.a); // __NOTE__ PASS AS PARAM

    // init
    GLuint newVAO;
    OCT_ID newID;
    OCT_index newIndex;
    iOCT_window* newWindow = (iOCT_window*)eOCT_pool_addEntry(&iOCT_windowSystem_inst.windowMPool.pool, &newIndex);
    newID = eOCT_IDMap_register(&iOCT_windowSystem_inst.windowMPool.IDMap, newIndex);
    newWindow->windowID = newID;
    newWindow->windowPtr = windowPtr;
    newWindow->targetResolution = (OCT_vec2){ (float)sizeX, (float)sizeY };
    newWindow->currentResolution = (OCT_vec2){ (float)sizeX, (float)sizeY };
    newWindow->activeCameraSourceEntity = OCT_HANDLE_NULL;
    newWindow->cameraUniformLocation = glGetUniformLocation(iOCT_renderer_inst.spriteShaderProgram, "cameraProj");

    glGenVertexArrays(1, &newVAO);
    newWindow->VAO = newVAO;
    OCT_handle windowHandle = {
        .containerID = OCT_ID_ECS,
        .objectID = newID,
    };

    iOCT_setupNewSpriteVAO(newWindow->VAO);
    glBindVertexArray(0);

    iOCT_windowSystem_inst.focusedWindowID = newID;
    printf("Created window %s %p\n", name, windowPtr);
    return windowHandle;
}

bool OCT_window_isOpen(OCT_handle windowHandle) {
    iOCT_window* window = (iOCT_window*)eOCT_getByID(&iOCT_windowSystem_inst.windowMPool.IDMap, &iOCT_windowSystem_inst.windowMPool.pool, windowHandle.objectID);
    if (!window) {
        return false;
    }
    return true;
}

bool OCT_window_anyOpen() {
    eOCT_pool* windowPool = &iOCT_windowSystem_inst.windowMPool.pool;
    return windowPool->count;
}

void iOCT_window_close(iOCT_window* window) {
    glfwDestroyWindow(window->windowPtr);
    eOCT_mappedPool_deleteEntry(&iOCT_windowSystem_inst.windowMPool, window->windowID);
}

void iOCT_window_poll(iOCT_window* window) {
    window->cursorDelta = OCT_vec2_zero;
    glfwPollEvents();
}

void iOCT_window_activate(iOCT_window window) {
    glfwMakeContextCurrent(window.windowPtr);
    glBindVertexArray(window.VAO);
}

OCT_mat3 iOCT_window_screenToWorld(iOCT_window window) {
    if (OCT_handle_isNULL(window.activeCameraSourceEntity)) {
        return OCT_mat3_identity;
    }
    iOCT_camera2D camera = *(iOCT_camera2D*)eOCT_entity_getComponentOnce(window.activeCameraSourceEntity, iOCT_renderer_inst.camera2DKey);
    OCT_mat3 entityGlobalTransform = *(OCT_mat3*)eOCT_entity_getFieldOnce(window.activeCameraSourceEntity, iOCT_renderer_inst.transform2DTicket);
    OCT_vec2 windowRes = window.currentResolution;

    OCT_vec2 toCameraScale = { 1.0f / window.currentResolution.x, -1.0f / window.currentResolution.y };
    OCT_vec2 toCameraTranslate = { -0.5f, 0.5f};
    OCT_mat3 screenToNDC = OCT_mat3_scale(OCT_mat3_identity, toCameraScale);
    screenToNDC = OCT_mat3_translate(screenToNDC, toCameraTranslate);

    OCT_mat3 cameraGlobal = OCT_mat3_mul(entityGlobalTransform, camera.cameraMatrix);

    OCT_mat3 twiceWorld = OCT_mat3_mul(cameraGlobal, screenToNDC);
    return twiceWorld;
}

OCT_mat3 iOCT_window_worldToNDC(iOCT_window window) {
    OCT_mat3 entityGlobalTransform = *(OCT_mat3*)eOCT_entity_getFieldOnce(window.activeCameraSourceEntity, iOCT_renderer_inst.transform2DTicket);
    iOCT_camera2D camera = *(iOCT_camera2D*)(eOCT_entity_getComponentOnce(window.activeCameraSourceEntity, iOCT_renderer_inst.camera2DKey));

    OCT_mat3 cameraGlobal = OCT_mat3_mul(entityGlobalTransform, camera.cameraMatrix);
    OCT_mat3 worldToCamera = OCT_mat3_inverse(cameraGlobal);
    OCT_vec2 NDCScale = {2, 2};

    OCT_mat3 final = OCT_mat3_scale(worldToCamera, NDCScale);
    return final;
}

// OCT_mat3 iOCT_window_getCameraOnlyProj(iOCT_window window) {
//     if (OCT_handle_isNULL(window.activeCameraSourceEntity)) {
//         return OCT_mat3_identity;
//     }
//     iOCT_camera2D camera = *(iOCT_camera2D*)eOCT_entity_getComponent(window.activeCameraSourceEntity, iOCT_renderer_inst.camera2DKey);
//
//     return camera.cameraMatrix;
// }
static void iOCT_window_setWorldResolution(OCT_vec2 XY) {

}

iOCT_window* iOCT_window_findByGLFWWindowPtr(GLFWwindow* windowPtr) {
    eOCT_pool* windowPool = &iOCT_windowSystem_inst.windowMPool.pool;
    for (OCT_index windowCtr = 0; windowCtr < windowPool->count; windowCtr++) {
        iOCT_window* window = (iOCT_window*)eOCT_pool_access(windowPool, windowCtr, 0);
        if (window->windowPtr ==  windowPtr) {
            return window;
        }
    }
    OCT_ERROR_LOG(OCT_EXIT_REFERENCE_DOES_NOT_EXIST, "No window has this windowPtr");
    return NULL;
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
