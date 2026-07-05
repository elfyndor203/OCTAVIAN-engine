#pragma once

#include "OCT_Math.h"
//
// void OCT_WDWModule_init(char* name, unsigned int sizeX, unsigned int sizeY, OCT_vec4 color);
// void OCT_WDWModule_poll();
// void OCT_WDWModule_update();
// bool OCT_window_closed();
//
// void OCT_WDWModule_free();

void eOCT_WINDOW_update_start(OCT_handle contextHandle);
void eOCT_WINDOW_update_finish(OCT_handle contextHandle);
