#pragma once

#include "OCT_Core_eng.h"

void system_register_RENDERER();
void eOCT_RENDERER_update(OCT_global contextHandle);

void system_register_WINDOW();
/*!
 * Checks if any windows should close. Clears all windows.
 */
void eOCT_WINDOW_startFrame();
/*!
 * Swaps all window buffers, displaying the new frame.
 */
void eOCT_WINDOW_finishFrame();