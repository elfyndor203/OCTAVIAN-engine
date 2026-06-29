#include "OCT_engine.h"
#include <stdio.h>
#include <stdbool.h>

#define NUM_WINDOWS 50

int main() {
	OCT_engine_init();

	OCT_handle mainContext;
	OCT_handle mainRoot = OCT_entityContext_open(&mainContext);

	OCT_handle child = OCT_entity_new(mainContext);
	OCT_transform2D_attach(child, mainRoot);

	OCT_handle windows[NUM_WINDOWS];
	windows[0] = OCT_window_open("wow", 1920, 1080, (OCT_vec4){0.0f, 0.0f, 0.0f, 1.0f});
	for (int i = 1; i < NUM_WINDOWS; i++) {
		windows[i] = OCT_window_open("HEY", 1080, 1080, (OCT_vec4){0.0f, 0.0f, 0.0f, 1.0f});
	}
	bool anyWindowOpen = true;

	while (anyWindowOpen) {
		anyWindowOpen = false;
		for (int i = 0; i < NUM_WINDOWS; i++) {
			if (OCT_window_isOpen(windows[i])) {
				anyWindowOpen = true;
			}
			else {
				windows[i] = (OCT_handle){0};
			}
		}

		OCT_engine_tick();
	}
	return 0;
}