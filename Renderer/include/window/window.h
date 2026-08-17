#pragma once
#include "OCT_Core.h"

void OCT_window_wipe();
void OCT_window_show();

OCT_global OCT_window_open(const char* name, unsigned int sizeX, unsigned int sizeY, OCT_vec4 color);
void OCT_window_setScreenSpaceZoom(OCT_global window, OCT_vec2 displayArea);
bool OCT_window_isOpen(OCT_global windowHandle);
bool OCT_window_anyOpen();