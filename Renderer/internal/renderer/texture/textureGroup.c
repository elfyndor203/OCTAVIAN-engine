#include "textureGroup_int.h"
#include "renderer/types_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "renderer/renderer_int.h"

#define TEXTURE_BASE_LEVEL 0
#define TEMP_MIPMAPS 1

OCT_handle OCT_textureGroup_open(OCT_vec2 dimensions, OCT_index maxCount) {
    OCT_ID systemID = iOCT_renderer_inst.systemDescription.systemID_reg;
    eOCT_IDMap texMap = eOCT_IDMap_init(systemID, maxCount);

    GLuint texArray;
    glGenTextures(1, &texArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texArray);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, TEXTURE_BASE_LEVEL, GL_RGBA8, (GLsizei)dimensions.x, (GLsizei)dimensions.y, (GLsizei)maxCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    // params in opengl wiki example
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    // add to texture group pool
    eOCT_pool* textureGroupPool = &iOCT_renderer_inst.textureGroupPool;
    eOCT_IDMap* textureGroupMap = &iOCT_renderer_inst.textureGroupMap;
    OCT_index newIndex;
    OCT_ID newID;
    iOCT_textureGroup* newTexGroup = eOCT_pool_addEntry(textureGroupPool, &newIndex);
    newID = eOCT_IDMap_register(textureGroupMap, newIndex);
    // set values
    newTexGroup->dimensions = dimensions;
    newTexGroup->glTexArray = texArray;
    newTexGroup->textureMap = texMap;
    newTexGroup->textureGroupID = newID;
    newTexGroup->textureCount = 0;

    // // update layers
    // OCT_index* layer;
    // for (OCT_index layerCtr = 0; layerCtr < iOCT_renderer_inst.layerCountPool.count; layerCtr++) {
    //     layer = (OCT_index*)eOCT_pool_access(&iOCT_renderer_inst.layerCountPool, layerCtr, 0);
    //     *layer += 1;
    // }
    OCT_handle newHandle = {
        .objectID = newID,
        .containerID = systemID,
    };

    printf("Created new texture group\n");

    return newHandle;
}

OCT_handle OCT_texture_new(OCT_handle textureGroup, const char* path) {
    const unsigned char* pixels = eOCT_image_load(path);
    if (!pixels) {
        perror("Can't open");
        OCT_ERROR_LOG(OCT_EXIT_FAILED_TO_OPEN_FILE, "Failed to load image");
        return OCT_HANDLE_NULL;
    }

    eOCT_IDMap* groupMap = &iOCT_renderer_inst.textureGroupMap;
    eOCT_pool* groupPool = &iOCT_renderer_inst.textureGroupPool;
    iOCT_textureGroup* texGroup = (iOCT_textureGroup*)eOCT_getByID(groupMap, groupPool, textureGroup.objectID);

    OCT_index newTexLayer = texGroup->textureCount++;
    glBindTexture(GL_TEXTURE_2D_ARRAY, texGroup->glTexArray);

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, TEXTURE_BASE_LEVEL,
                    0, 0, (GLsizei)newTexLayer,
                    (GLsizei)texGroup->dimensions.x, (GLsizei)texGroup->dimensions.y, 1,
                    GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    OCT_ID newTexID = eOCT_IDMap_register(&texGroup->textureMap, newTexLayer);

    OCT_handle newHandle = {
        .objectID = newTexID,
        .containerID = texGroup->textureGroupID,
    };

    printf("Loaded new texture\n");

    return newHandle;
}