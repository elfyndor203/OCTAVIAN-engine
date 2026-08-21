#include "interactBox2D_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"

#include "inputSystem_int.h"

void OCT_interactBox2D_attach(OCT_local entity, OCT_shape2 shape, OCT_index layer) {
    iOCT_interactBox2D newBox = {
        .entity = entity,
        .shape = shape,
        .layer = layer,
        .interactTime = 0
    };
    eOCT_entity_attachComponent(entity, iOCT_inputSystem_inst.interactBoxKey, &newBox, NULL);
}