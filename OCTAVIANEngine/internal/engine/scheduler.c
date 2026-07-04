
#include "OCT_Core_eng.h"
#include "OCT_window.h"
#include "OCT_Renderer.h"

void OCT_engine_update(OCT_handle context) {
    system_update_WINDOW(context);
    system_update_RENDERER(context);
}
