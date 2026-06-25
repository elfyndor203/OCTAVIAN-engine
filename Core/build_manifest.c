#pragma once

extern void testDesc_register();
extern void systemB_register();
extern void systemC_register();
extern void systemD_register();

void iOCT_registerAllSystems() {
	testDesc_register();
	systemB_register();
	systemC_register();
	systemD_register();
};