
#include "OCT_Core_eng.h"
#include "systemDescription_platform.h"
#include "systemDescription_input.h"
#include "systemDescription_physics.h"
#include "systemDescription_renderer.h"
#include "systemDescription_world.h"

void OCT_engine_frame_start() {
    eOCT_WINDOW_startFrame();
    eOCT_globals_update();

    eOCT_PLATFORM_update();
    eOCT_INPUT_update();
}
void OCT_engine_updateContext(OCT_global context) {
    eOCT_entityContext_prepare(context);

    eOCT_WORLD_update(context);
    eOCT_PHYSICS_update(context);
    eOCT_RENDERER_update(context);
}

void OCT_engine_frame_show() {
    eOCT_INPUT_clear();
    eOCT_WINDOW_finishFrame();
}