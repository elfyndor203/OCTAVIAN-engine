#pragma once

void testDesc_register();
void systemB_register();
void systemC_register();
void systemD_register();
void system_register_NAME();

void iOCT_registerAllSystems() {
	testDesc_register();
	systemB_register();
	systemC_register();
	systemD_register();
	system_register_NAME();
};