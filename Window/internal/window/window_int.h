#pragma once
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct iOCT_window {
    GLFWwindow* windowPtr;
    OCT_vec2 targetResolution;
    OCT_vec2 currentResolution;

    bool open;

    OCT_vec2 cursorDelta;
};

void iOCT_window_wipe();
void iOCT_window_show();
void iOCT_window_viewport(int width, int height);

void iOCT_window_callback_resize(GLFWwindow* window, int newWidth, int newHeight);

void iOCT_window_poll(iOCT_window* window);