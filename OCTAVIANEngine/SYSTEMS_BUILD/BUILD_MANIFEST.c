#include "systems_build.h"

#include "systemDescription_sysEx.h"
#include "systemDescription_world.h"
#include "systemDescription_window.h"

void init_OCT_registerAllSystems() {
	// testDesc_register();
	// systemB_register();
	// systemC_register();
	// systemD_register();
	system_register_systemEx();
	system_register_WORLD();
	system_register_WINDOW();
};

void OCT_engine_tick() {
	system_update_WINDOW();
}