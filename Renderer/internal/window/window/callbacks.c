#include "window_int.h"
#include "window/types_int.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "window/windowSystem_int.h"
#include "window/inputs/inputs_int.h"

void iOCT_window_keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers) {
    if (!(action == GLFW_PRESS || action == GLFW_RELEASE)) {
        return;
    }

    iOCT_keyEvent keyEvent = {
        .key = key,
        .pressed = (action == GLFW_PRESS),
        .released = (action == GLFW_RELEASE),
    };

    eOCT_event_broadcast(iOCT_windowSystem_inst.keyEventCache, OCT_HANDLE_NULL, &keyEvent);
}

void iOCT_window_resizeCallback(GLFWwindow* window, int width, int height) {}