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

    eOCT_fieldDescription glfwKeyEvent = {
        .name = "glfwKeys",
        .provider = eOCT_FIELDPROVIDER_DATAPOOL,
        .offset = offsetof(iOCT_keyEvent, key),
        .type = eOCT_FIELDTYPE_INT64
    };
    eOCT_fieldDescription glfwKeyAction = {
        .name = "glfwKeyAction",
        .provider = eOCT_FIELDPROVIDER_DATAPOOL,
        .offset = 0,
        .type = eOCT_FIELDTYPE_INT64
    };
    eOCT_fieldDescription keyFields[] = { glfwKeyEvent, glfwKeyAction };
    eOCT_dataPoolDescription glfwKeyData = {
        .name = "glfwKeyData",
        .providedFields = eOCT_generateFieldDescriptionPool(keyFields, 2),
        .global = true,
        .stride = sizeof(int),
        .cacheLocation = &iOCT_windowSystem_inst.keyCache
    };

    eOCT_dataPoolDescription dataPools[] = { window, glfwKeyData };

    eOCT_systemDescription windowSystem = {
        .name = "Window",
        .providedDataPools = eOCT_generateDataPoolDescriptionPool(dataPools, 2),
        .providedComponents = eOCT_POOL_EMPTY,
        .requestedFields = eOCT_POOL_EMPTY,
        .initFx = system_init_WINDOW
    };

    iOCT_windowSystem_inst.windowSystem = windowSystem;
    eOCT_registry_registerSystem(&iOCT_windowSystem_inst.windowSystem);
}
