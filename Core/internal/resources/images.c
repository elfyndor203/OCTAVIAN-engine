#include "images_int.h"
#include "types_int.h"

#include "stb_image.h"
#include <string.h>

#include "layout/types.h"
#include "utilities/pools_eng.h"
#include "resources/resources_int.h"

const unsigned char* eOCT_image_load(const char* path) {
    stbi_set_flip_vertically_on_load(1);

    OCT_index newIndex;
    OCT_ID newID;
    iOCT_image* newImage;

    newImage = eOCT_pool_addEntry(&iOCT_resources_inst.images, &newIndex);
    newID = eOCT_IDMap_register(&iOCT_resources_inst.resourceMap, newIndex);

    strncpy(newImage->path, path, iOCT_RESOURCE_PATHNAME_MAX - 1);
    newImage->path[iOCT_RESOURCE_PATHNAME_MAX - 1] = '\0';

    int width;
    int height;
    int channels;
    unsigned char* pixels = stbi_load(path, &width, &height, &channels, iOCT_RGBA);
    if (!pixels) {
        printf("Failed load\n");
        printf(stbi_failure_reason());
        return NULL;
    }

    printf("Loaded image from %s\n", path);
    newImage->pixels = pixels;
    return pixels;
}