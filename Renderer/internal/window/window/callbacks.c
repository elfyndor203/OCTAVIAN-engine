#include "window_int.h"
#include "window/types_int.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "window/windowSystem_int.h"

void iOCT_window_keyCallback(GLFWwindow* window, int key, int scancode, int action, int modifiers) {
    if (!(action == GLFW_PRESS || action == GLFW_RELEASE)) {
        return;
    }
    iOCT_keyEvent* destination = (iOCT_keyEvent*)eOCT_addGlobalDataEntry(iOCT_windowSystem_inst.keyCache, false, NULL);
    destination->key = key;

    if (action == GLFW_PRESS) {
        destination->pressed = true;
        //printf("Key %c pressed\n", key);
    }
    else {
        destination->pressed = false;
        //printf("Key %c released\n", key);
    }
}

void iOCT_window_resizeCallback(GLFWwindow* window, int width, int height) {}