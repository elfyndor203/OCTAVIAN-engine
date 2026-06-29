#pragma once
#include "OCT_Core_eng.h"

void OCT_window_wipe();
void OCT_window_show();

OCT_handle OCT_window_open(const char* name, unsigned int sizeX, unsigned int sizeY, OCT_vec4 color);
bool OCT_window_isOpen(OCT_handle windowHandle);