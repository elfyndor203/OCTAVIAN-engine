#pragma once

#include "OCT_Core_eng.h"

void system_register_RENDERER();
void eOCT_RENDERER_update(OCT_handle contextHandle);

void system_register_WINDOW();
void eOCT_WINDOW_update_start(OCT_handle contextHandle);
void eOCT_WINDOW_update_finish(OCT_handle contextHandle);