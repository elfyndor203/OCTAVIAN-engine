#include "OCT_engine.h"
#include <stdio.h>

int main() {
	OCT_engine_init();
	OCT_handle mainContext;
	OCT_handle mainRoot = OCT_entityContext_open(&mainContext);

	printf("Got here\n");
	OCT_handle child = OCT_entity_new(mainContext);
	OCT_transform2D_attach(child, mainRoot);

	OCT_entityContext_dumpEntityPool(mainContext);
	return 0;
}