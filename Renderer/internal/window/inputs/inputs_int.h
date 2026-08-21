#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "OCT_Core_eng.h"

#define iOCT_GLFW_MOUSE_BUTTONS_OFFSET (GLFW_KEY_LAST + 1)

void iOCT_buttonList_init();
OCT_BUTTON iOCT_getOCTButtonFromGLFW(int glfwKeyCode, bool mouse);
OCT_vec2 iOCT_cursor_calcPosContext(OCT_global context);

// void iOCT_keyMap_init();
// void iOCT_mouseMap_init();
// void iOCT_window_callback_keyEvent(GLFWwindow* window, int key, int scancode, int action, int modifiers);
// void iOCT_window_callback_mouseEvent(GLFWwindow* window, int key, int action, int modifiers);
// void iOCT_window_callback_mouseMove(GLFWwindow* window, double x, double y);
// void iOCT_window_callback_mouseScroll(GLFWwindow* window, double xOffset, double yOffset);