#include <stdio.h>

#include "renderer_int.h"
#include "renderer/types_int.h"

#include <string.h>

#include "textureGroup_int.h"
#include "sprite2D_int.h"

iOCT_renderer iOCT_renderer_inst = { 0 };

static OCT_index iOCT_renderer_findMaxLayerSize(OCT_index* layerOut);

void iOCT_renderer_init() {
    OCT_ID systemID = iOCT_renderer_inst.systemDescription.systemID_reg;
    iOCT_renderer_inst.textureGroupPool = eOCT_pool_init(systemID, eOCT_POOL_SIZE_DEFAULT, sizeof(iOCT_textureGroup));
    iOCT_renderer_inst.textureGroupMap = eOCT_IDMap_init(systemID, eOCT_POOL_SIZE_DEFAULT);
    iOCT_renderer_inst.layerCountsTotalsPool = eOCT_pool_init(systemID, eOCT_POOL_SIZE_DEFAULT, sizeof(OCT_index));
    iOCT_renderer_inst.spriteFullDataBuffer = eOCT_pool_init(systemID, eOCT_POOL_SIZE_DEFAULT,sizeof(iOCT_spriteFullData));
    iOCT_renderer_inst.drawOrderPool = eOCT_pool_init(systemID, eOCT_POOL_SIZE_DEFAULT, sizeof(OCT_index));
}

// void iOCT_renderer_addToLayer(iOCT_textureGroup* texGroup, OCT_index newSpriteLayer) {
//     eOCT_pool* layerCountPool = &texGroup->layerCountsPool;
//
//     OCT_index* spriteEntry;
//     if (layerCountPool->count <= newSpriteLayer) {
//         while (layerCountPool->count < newSpriteLayer) {
//             spriteEntry = (OCT_index*)eOCT_pool_addEntry(layerCountPool, NULL);
//             *spriteEntry = 0;
//         }
//         spriteEntry = (OCT_index*)eOCT_pool_addEntry(layerCountPool, NULL);
//         *spriteEntry = 1;
//     }
//     else {
//         spriteEntry = (OCT_index*)eOCT_pool_access(layerCountPool, newSpriteLayer, 0);
//         *spriteEntry += 1;
//     }
// }

void iOCT_renderer_drawAll(OCT_handle contextHandle) {
    eOCT_pool* spritePool = eOCT_getComponentPool(contextHandle, iOCT_renderer_inst.sprite2DCache);
    eOCT_pool* transformPool = eOCT_getFieldSourcePool(contextHandle, iOCT_renderer_inst.transform2DCache);
    iOCT_sprite2D* spriteArray = (iOCT_sprite2D*)spritePool->array;
    OCT_mat3* transformArray = transformPool->array;

    eOCT_pool* spriteBufferPool = &iOCT_renderer_inst.spriteFullDataBuffer;
    eOCT_pool_expand(spriteBufferPool, spritePool->count);
    eOCT_pool* texGroupPool = &iOCT_renderer_inst.textureGroupPool;
    eOCT_pool* layerCountsTotalsPool = &iOCT_renderer_inst.layerCountsTotalsPool;
    eOCT_IDMap* texGroupMap = &iOCT_renderer_inst.textureGroupMap;

    // batch sprites by layer, then textureGroup
    iOCT_sprite2D* spriteListGrouped[spritePool->count];
    OCT_index biggestLayerCt = iOCT_renderer_findMaxLayerSize(NULL);
    OCT_index spritesPerTexGroupPerLayer[spritePool->count];

    //__NOTE__ FIX COUNTING LOGIC
    // create a list of locations of each layer in the array
    OCT_index layerPositionCtrs[layerCountsTotalsPool->count];
    OCT_index layerCurrentPosition = 0;
    for (OCT_index layerCtr = 0; layerCtr < layerCountsTotalsPool->count; layerCtr++) {
        layerPositionCtrs[layerCtr] = *(OCT_index*)eOCT_pool_access(layerCountsTotalsPool, layerCtr, 0) + layerCurrentPosition;
        layerCurrentPosition += layerPositionCtrs[layerCtr];
        printf("Layer %zu position: %zu", layerCtr, layerCurrentPosition);
    }
    // Create a list of the local positions of each texGroup in the array (local to the layer location)
    iOCT_textureGroup* texGroup;
    OCT_index texPositionCtrs[texGroupPool->count][layerCountsTotalsPool->count];
    OCT_index texGroupCurrentPosition = 0;
    for (OCT_index texGroupCtr = 0; texGroupCtr < texGroupPool->count; texGroupCtr++) {
        texGroup = (iOCT_textureGroup*)eOCT_pool_access(texGroupPool, texGroupCtr, 0);
        for (OCT_index layerCtr = 0; layerCtr < layerCountsTotalsPool->count; layerCtr++) {
            texPositionCtrs[layerCtr][texGroupCtr] = *(OCT_index*)eOCT_pool_access(&texGroup->layerCountsPool, layerCtr, 0) + texGroupCurrentPosition;
            texGroupCurrentPosition += texPositionCtrs[texGroupCtr][layerCtr];
            printf("TexGroup %zu layer %zu position %zu", texGroupCtr, layerCtr, texGroupCurrentPosition);
        }
    }
    //__NOTE FIX COUNTING LOGIC

    // place the sprite data in the buffer
    iOCT_sprite2D sprite;
    OCT_index spriteLayer;
    OCT_index spriteTexGroup;
    OCT_index entityTransformIndex;
    OCT_mat3 entityTransform;

    OCT_index layerPosition;
    OCT_index texLocalPosition;

    iOCT_spriteFullData* bufferDestination;

    for (OCT_index spriteCtr = 0; spriteCtr < spritePool->count; spriteCtr++) {
        sprite = spriteArray[spriteCtr];
        spriteLayer = sprite.drawLayer;
        spriteTexGroup = eOCT_IDMap_getIndex(texGroupMap, sprite.texGroupID);

        entityTransform = *(OCT_mat3*)eOCT_entity_getField(sprite.entity, iOCT_renderer_inst.transform2DCache);

        layerPosition = layerPositionCtrs[spriteLayer]++;
        texLocalPosition = texPositionCtrs[spriteLayer][spriteTexGroup]++;
        bufferDestination = (iOCT_spriteFullData*)spriteBufferPool->array + layerPosition + texLocalPosition;
        bufferDestination->spriteData = sprite.spriteData;
        bufferDestination->transform = entityTransform;
    }
}

static OCT_index iOCT_renderer_findMaxLayerSize(OCT_index* layerOut) {
    OCT_index* layer;
    OCT_index biggestLayer = 0;
    OCT_index max = 0;
    for (OCT_index layerCtr = 0; layerCtr < iOCT_renderer_inst.layerCountsTotalsPool.count; layerCtr++) {
        layer = (OCT_index*)eOCT_pool_access(&iOCT_renderer_inst.layerCountsTotalsPool, layerCtr, 0);
        if (*layer > max) {
            max = *layer;
            biggestLayer = layerCtr;
        }
    }
    if (layerOut) {
        *layerOut = biggestLayer;
    }
    return max;
}

