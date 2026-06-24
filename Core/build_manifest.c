#pragma once

extern void testDesc_register();
extern void systemB_register();
extern void systemC_register_fails();
extern void systemD_register_fails();

void iOCT_registerAllSystems() {
	testDesc_register();
	systemB_register();
	systemC_register_fails();
	systemD_register_fails();
};