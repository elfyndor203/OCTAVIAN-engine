
#include "OCT_Core_eng.h"
#include "OCT_window.h"
#include "OCT_Renderer.h"

void OCT_engine_update(OCT_handle context) {
    eOCT_WINDOW_update_start(context);
    eOCT_RENDERER_update(context);
    eOCT_WINDOW_update_finish(context);
}
