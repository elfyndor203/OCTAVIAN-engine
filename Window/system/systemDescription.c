#include "systemDescription_window.h"

#include "OCT_Core_eng.h"

#include "window/window_int.h"
#include "windowSystem/windowSystem_int.h"

void system_register_WINDOW() {
    eOCT_dataPoolDescription window = {
        .name = "Window",
        .stride = sizeof(iOCT_window),
        .providedFields = eOCT_POOL_EMPTY,
        .global = true,
        .cacheLocation = &iOCT_windowSystem_inst.windowCache
    };

    eOCT_dataPoolDescription dataPools[] = { window };

    eOCT_systemDescription windowSystem = {
        .name = "Window",
        .providedDataPools = eOCT_generateDataPoolDescriptionPool(dataPools, 1),
        .providedComponents = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_POOL_EMPTY,
        .initFx = iOCT_windowSystem_init
    };

    iOCT_windowSystem_inst.windowSystem = windowSystem;
    eOCT_registry_registerSystem(&iOCT_windowSystem_inst.windowSystem);
}
