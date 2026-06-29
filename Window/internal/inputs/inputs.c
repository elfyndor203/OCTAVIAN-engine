#pragma once
#include "inputs/inputs_internal.h"

#include "cOCT_Communication.h"
#include "cOCT_EngineStructure.h"
#include "OCT_Errors.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include "windowSystem/WDWModule_internal.h"

static OCT_key iOCT_getOCTKey(int GLFW_KEY);
static OCT_key iOCT_getOCTButton(int glfwButton);

void iOCT_keyMap_init() {
	iOCT_WDWModule_instance.keyMap = (OCT_key*)calloc((GLFW_KEY_LAST + 1), sizeof(OCT_key));
    OCT_key* map = iOCT_WDWModule_instance.keyMap;

    if (!map) {
        OCT_logError(EXIT_FAILED_TO_ALLOCATE);
        return;
    }

    map[GLFW_KEY_A] = OCT_KEY_A;
    map[GLFW_KEY_B] = OCT_KEY_B;
    map[GLFW_KEY_C] = OCT_KEY_C;
    map[GLFW_KEY_D] = OCT_KEY_D;
    map[GLFW_KEY_E] = OCT_KEY_E;
    map[GLFW_KEY_F] = OCT_KEY_F;
    map[GLFW_KEY_G] = OCT_KEY_G;
    map[GLFW_KEY_H] = OCT_KEY_H;
    map[GLFW_KEY_I] = OCT_KEY_I;
    map[GLFW_KEY_J] = OCT_KEY_J;
    map[GLFW_KEY_K] = OCT_KEY_K;
    map[GLFW_KEY_L] = OCT_KEY_L;
    map[GLFW_KEY_M] = OCT_KEY_M;
    map[GLFW_KEY_N] = OCT_KEY_N;
    map[GLFW_KEY_O] = OCT_KEY_O;
    map[GLFW_KEY_P] = OCT_KEY_P;
    map[GLFW_KEY_Q] = OCT_KEY_Q;
    map[GLFW_KEY_R] = OCT_KEY_R;
    map[GLFW_KEY_S] = OCT_KEY_S;
    map[GLFW_KEY_T] = OCT_KEY_T;
    map[GLFW_KEY_U] = OCT_KEY_U;
    map[GLFW_KEY_V] = OCT_KEY_V;
    map[GLFW_KEY_W] = OCT_KEY_W;
    map[GLFW_KEY_X] = OCT_KEY_X;
    map[GLFW_KEY_Y] = OCT_KEY_Y;
    map[GLFW_KEY_Z] = OCT_KEY_Z;

    // numbers
    map[GLFW_KEY_0] = OCT_KEY_0;
    map[GLFW_KEY_1] = OCT_KEY_1;
    map[GLFW_KEY_2] = OCT_KEY_2;
    map[GLFW_KEY_3] = OCT_KEY_3;
    map[GLFW_KEY_4] = OCT_KEY_4;
    map[GLFW_KEY_5] = OCT_KEY_5;
    map[GLFW_KEY_6] = OCT_KEY_6;
    map[GLFW_KEY_7] = OCT_KEY_7;
    map[GLFW_KEY_8] = OCT_KEY_8;
    map[GLFW_KEY_9] = OCT_KEY_9;

    // function keys
    map[GLFW_KEY_F1] = OCT_KEY_F1;
    map[GLFW_KEY_F2] = OCT_KEY_F2;
    map[GLFW_KEY_F3] = OCT_KEY_F3;
    map[GLFW_KEY_F4] = OCT_KEY_F4;
    map[GLFW_KEY_F5] = OCT_KEY_F5;
    map[GLFW_KEY_F6] = OCT_KEY_F6;
    map[GLFW_KEY_F7] = OCT_KEY_F7;
    map[GLFW_KEY_F8] = OCT_KEY_F8;
    map[GLFW_KEY_F9] = OCT_KEY_F9;
    map[GLFW_KEY_F10] = OCT_KEY_F10;
    map[GLFW_KEY_F11] = OCT_KEY_F11;
    map[GLFW_KEY_F12] = OCT_KEY_F12;

    // navigation
    map[GLFW_KEY_UP] = OCT_KEY_UP;
    map[GLFW_KEY_DOWN] = OCT_KEY_DOWN;
    map[GLFW_KEY_LEFT] = OCT_KEY_LEFT;
    map[GLFW_KEY_RIGHT] = OCT_KEY_RIGHT;
    map[GLFW_KEY_HOME] = OCT_KEY_HOME;
    map[GLFW_KEY_END] = OCT_KEY_END;
    map[GLFW_KEY_PAGE_UP] = OCT_KEY_PAGE_UP;
    map[GLFW_KEY_PAGE_DOWN] = OCT_KEY_PAGE_DOWN;
    map[GLFW_KEY_INSERT] = OCT_KEY_INSERT;
    map[GLFW_KEY_DELETE] = OCT_KEY_DELETE;

    // modifiers
    map[GLFW_KEY_LEFT_SHIFT] = OCT_KEY_LEFT_SHIFT;
    map[GLFW_KEY_RIGHT_SHIFT] = OCT_KEY_RIGHT_SHIFT;
    map[GLFW_KEY_LEFT_CONTROL] = OCT_KEY_LEFT_CTRL;
    map[GLFW_KEY_RIGHT_CONTROL] = OCT_KEY_RIGHT_CTRL;
    map[GLFW_KEY_LEFT_ALT] = OCT_KEY_LEFT_ALT;
    map[GLFW_KEY_RIGHT_ALT] = OCT_KEY_RIGHT_ALT;
    map[GLFW_KEY_LEFT_SUPER] = OCT_KEY_LEFT_SUPER;
    map[GLFW_KEY_RIGHT_SUPER] = OCT_KEY_RIGHT_SUPER;
    map[GLFW_KEY_CAPS_LOCK] = OCT_KEY_CAPS_LOCK;
    map[GLFW_KEY_NUM_LOCK] = OCT_KEY_NUM_LOCK;
    map[GLFW_KEY_SCROLL_LOCK] = OCT_KEY_SCROLL_LOCK;

    // whitespace and editing
    map[GLFW_KEY_SPACE] = OCT_KEY_SPACE;
    map[GLFW_KEY_ENTER] = OCT_KEY_ENTER;
    map[GLFW_KEY_BACKSPACE] = OCT_KEY_BACKSPACE;
    map[GLFW_KEY_TAB] = OCT_KEY_TAB;
    map[GLFW_KEY_ESCAPE] = OCT_KEY_ESCAPE;

    // punctuation
    map[GLFW_KEY_APOSTROPHE] = OCT_KEY_APOSTROPHE;
    map[GLFW_KEY_COMMA] = OCT_KEY_COMMA;
    map[GLFW_KEY_MINUS] = OCT_KEY_MINUS;
    map[GLFW_KEY_PERIOD] = OCT_KEY_PERIOD;
    map[GLFW_KEY_SLASH] = OCT_KEY_SLASH;
    map[GLFW_KEY_SEMICOLON] = OCT_KEY_SEMICOLON;
    map[GLFW_KEY_EQUAL] = OCT_KEY_EQUAL;
    map[GLFW_KEY_LEFT_BRACKET] = OCT_KEY_LEFT_BRACKET;
    map[GLFW_KEY_BACKSLASH] = OCT_KEY_BACKSLASH;
    map[GLFW_KEY_RIGHT_BRACKET] = OCT_KEY_RIGHT_BRACKET;
    map[GLFW_KEY_GRAVE_ACCENT] = OCT_KEY_GRAVE;

    // numpad
    map[GLFW_KEY_KP_0] = OCT_KEY_NP_0;
    map[GLFW_KEY_KP_1] = OCT_KEY_NP_1;
    map[GLFW_KEY_KP_2] = OCT_KEY_NP_2;
    map[GLFW_KEY_KP_3] = OCT_KEY_NP_3;
    map[GLFW_KEY_KP_4] = OCT_KEY_NP_4;
    map[GLFW_KEY_KP_5] = OCT_KEY_NP_5;
    map[GLFW_KEY_KP_6] = OCT_KEY_NP_6;
    map[GLFW_KEY_KP_7] = OCT_KEY_NP_7;
    map[GLFW_KEY_KP_8] = OCT_KEY_NP_8;
    map[GLFW_KEY_KP_9] = OCT_KEY_NP_9;
    map[GLFW_KEY_KP_DECIMAL] = OCT_KEY_NP_DECIMAL;
    map[GLFW_KEY_KP_DIVIDE] = OCT_KEY_NP_DIVIDE;
    map[GLFW_KEY_KP_MULTIPLY] = OCT_KEY_NP_MULTIPLY;
    map[GLFW_KEY_KP_SUBTRACT] = OCT_KEY_NP_SUBTRACT;
    map[GLFW_KEY_KP_ADD] = OCT_KEY_NP_ADD;
    map[GLFW_KEY_KP_ENTER] = OCT_KEY_NP_ENTER;

}

void iOCT_mouseMap_init() {
    iOCT_WDWModule_instance.mouseMap = (OCT_key*)calloc((GLFW_MOUSE_BUTTON_LAST + 1), sizeof(OCT_key));
    OCT_key* map = iOCT_WDWModule_instance.mouseMap;

    if (!map) {
        OCT_logError(EXIT_FAILED_TO_ALLOCATE);
        return;
    }

    map[GLFW_MOUSE_BUTTON_1] = OCT_KEY_MOUSE_LEFT;
    map[GLFW_MOUSE_BUTTON_2] = OCT_KEY_MOUSE_RIGHT;
    map[GLFW_MOUSE_BUTTON_3] = OCT_KEY_MOUSE_MIDDLE;
    map[GLFW_MOUSE_BUTTON_4] = OCT_KEY_MOUSE_4;
    map[GLFW_MOUSE_BUTTON_5] = OCT_KEY_MOUSE_5;
}

void iOCT_window_keyEvent(OCT_key key, int action, int modifiers) {
    cOCT_message keyMsg;
    if (action == GLFW_PRESS) {
        keyMsg.messageType = cOCT_MSG_KEY_DOWN;
    }
    else if (action == GLFW_RELEASE) {
        keyMsg.messageType = cOCT_MSG_KEY_UP;
    }
    else {
        return;
    }
    keyMsg.key_event.key = key;

    cOCT_message_push(OCT_subsystem_input, keyMsg, cOCT_INBOX);
}

OCT_vec2 _OCT_window_cursorPos_get() {
    return iOCT_WDWModule_instance.cursorPos;
}
OCT_vec2 _OCT_window_cursorDelta_get() {
    return iOCT_WDWModule_instance.cursorDelta;
}

OCT_vec2 _OCT_window_scrollDelta_get() {
    return iOCT_WDWModule_instance.scrollDelta;
}

#pragma region helpers
static OCT_key iOCT_getOCTKey(int glfwKey) {
    return iOCT_WDWModule_instance.keyMap[glfwKey];
}
static OCT_key iOCT_getOCTButton(int glfwButton) {
    return iOCT_WDWModule_instance.mouseMap[glfwButton];
}
#pragma endregion

# pragma region callback
void iOCT_window_callback_keyEvent(GLFWwindow* window, int key, int scancode, int action, int modifiers) {
    iOCT_window_keyEvent(iOCT_getOCTKey(key), action, modifiers);
}

void iOCT_window_callback_mouseEvent(GLFWwindow* window, int key, int action, int modifiers) {
    iOCT_window_keyEvent(iOCT_getOCTButton(key), action, modifiers);
}

void iOCT_window_callback_mouseMove(GLFWwindow* window, double x, double y) {
    OCT_vec2 prevPos = iOCT_WDWModule_instance.cursorPos;
    iOCT_WDWModule_instance.cursorPos = (OCT_vec2){ x, y };
    iOCT_WDWModule_instance.cursorDelta = OCT_vec2_sub(iOCT_WDWModule_instance.cursorPos, prevPos);
    //printf("Delta x: %lf, y: %lf\n", iOCT_WDWModule_instance.cursorDelta.x, iOCT_WDWModule_instance.cursorDelta.y);
}

void iOCT_window_callback_mouseScroll(GLFWwindow* window, double xOffset, double yOffset) {
	OCT_vec2 scrollDelta = { xOffset, yOffset };
	iOCT_WDWModule_instance.scrollDelta = scrollDelta;
}
#pragma endregion