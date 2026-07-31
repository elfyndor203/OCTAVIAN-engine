#include "systems_build.h"

#include "systemDescription_sysEx.h"
#include "systemDescription_world.h"
#include "systemDescription_renderer.h"
#include "systemDescription_input.h"

void init_OCT_registerAllSystems() {
	system_register_systemEx();

	system_register_WORLD();
	system_register_WINDOW();
	system_register_RENDERER();
	system_register_INPUT();
};