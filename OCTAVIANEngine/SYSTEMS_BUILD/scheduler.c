
#include "OCT_Core_eng.h"
#include "systemDescription_renderer.h"
#include "systemDescription_world.h"

void OCT_engine_frame_start() {
    eOCT_WINDOW_startFrame();
}
void OCT_engine_update(OCT_handle context) {
    eOCT_WORLD_update(context);
    eOCT_RENDERER_update(context);
}

void OCT_engine_frame_show() {
    eOCT_WINDOW_finishFrame();
}