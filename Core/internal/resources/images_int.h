#pragma once
#include "resources/resources_eng.h"
#include "resources/types_eng.h"

#include "OCT_Core_eng.h"
#include <stdbool.h>

#define iOCT_RESOURCE_PATHNAME_MAX 255
#define iOCT_RGBA 4

struct iOCT_image {
    char path[iOCT_RESOURCE_PATHNAME_MAX];
    unsigned char* pixels;

    bool loaded;
    OCT_index uses;
};
