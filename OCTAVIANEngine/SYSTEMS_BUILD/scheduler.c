
#include "OCT_Core_eng.h"
#include "systemDescription_window.h"
#include "systemDescription_renderer.h"
#include "systemDescription_world.h"

void OCT_engine_update(OCT_handle context) {
    eOCT_WINDOW_update_start(context);
    eOCT_WORLD_update(context);
    eOCT_RENDERER_update(context);
    eOCT_WINDOW_update_finish(context);
}
