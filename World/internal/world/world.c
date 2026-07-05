#include "world_int.h"
#include "types_int.h"

#include "transform2D/transform2D_int.h"

iOCT_world iOCT_world_inst = { 0 };

void iOCT_world_init() {
}

void eOCT_WORLD_update(OCT_handle contextHandle) {
    eOCT_pool* transformPool = (eOCT_pool*)eOCT_getComponentPool(contextHandle, iOCT_world_inst.transform2DCache);
    iOCT_transform2D* transformArray = (iOCT_transform2D*)transformPool->array;

    for (OCT_index transformCtr = 0; transformCtr < transformPool->count; transformCtr++) {
        iOCT_transform2D* transform = &transformArray[transformCtr];

        transform->localMatrix = OCT_mat3_generate(transform->position, transform->scale, transform->rotation);
        transform->globalMatrix = transform->localMatrix; //__NOTE__ DO PARENTING __
    }
}