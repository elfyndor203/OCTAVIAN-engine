#include "resources_int.h"
#include "types_int.h"

#include "utilities/utilities_eng.h"
#include "layout/systems.h"
#include <stdio.h>

#include "images_int.h"

iOCT_resources iOCT_resources_inst = { 0 };

void init_OCT_resources_init() {
    iOCT_resources_inst.resourceMap = eOCT_IDMap_init(OCT_ID_RESOURCES, eOCT_POOL_CAPACITY_DEFAULT);
    iOCT_resources_inst.images = eOCT_pool_open(OCT_ID_RESOURCES, eOCT_POOL_CAPACITY_DEFAULT, sizeof(iOCT_image));

    printf("| Resources initialized\n");
}