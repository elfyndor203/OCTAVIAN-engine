#include "inputs_int.h"

#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

OCT_vec2 OCT_mouse_readPos() {
    return iOCT_inputSystem_inst.mousePosition;
}