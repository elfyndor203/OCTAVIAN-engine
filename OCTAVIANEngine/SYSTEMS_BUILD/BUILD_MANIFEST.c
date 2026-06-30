#include "systems_build.h"

#include "systemDescription_sysEx.h"
#include "systemDescription_world.h"
#include "systemDescription_window.h"
#include "systemDescription_renderer.h"

void init_OCT_registerAllSystems() {
	// testDesc_register();
	// systemB_register();
	// systemC_register();
	// systemD_register();
	system_register_systemEx();
	system_register_WORLD();
	system_register_WINDOW();
	system_register_RENDERER();
};

void OCT_engine_tick() {
	system_update_WINDOW();
}