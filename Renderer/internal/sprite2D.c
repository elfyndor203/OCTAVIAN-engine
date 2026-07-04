#include "sprite2D_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>

#include "renderer_int.h"
#include "textureGroup_int.h"

uint64_t generateSortKey(OCT_index drawLayer, OCT_index texGroupIndex);

void OCT_sprite2D_attach(OCT_handle entity, OCT_handle texture, OCT_vec4 uv, OCT_vec4 color, OCT_vec2 dimensions, OCT_index drawLayer) {
    OCT_index texGroupIndex = eOCT_IDMap_getIndex(&iOCT_renderer_inst.textureGroupMap, texture.containerID);
    iOCT_sprite2D* newSprite = eOCT_entity_attachComponent(entity, iOCT_renderer_inst.sprite2DCache);
    newSprite->entity = entity;
    newSprite->texGroupID = texture.containerID;
    newSprite->texID = texture.objectID;
    newSprite->spriteTransform = OCT_mat3_identity;
    // resolve spriteData texArrayLayer at draw time
    newSprite->spriteData.uv = uv;
    newSprite->spriteData.color = color;
    newSprite->spriteData.dimensions = dimensions;
    newSprite->sortKey = generateSortKey(drawLayer, texGroupIndex);

    printf("Attached sprite2D to entity %zu\n", entity.objectID);

    iOCT_textureGroup* texGroup = (iOCT_textureGroup*)eOCT_getByID(&iOCT_renderer_inst.textureGroupMap, &iOCT_renderer_inst.textureGroupPool, texture.containerID);
}

/// assumes layer and texGroup don't exceed 32 bit max, because it'd better not
uint64_t generateSortKey(OCT_index drawLayer, OCT_index texGroupIndex) {
    assert(drawLayer <= UINT32_MAX);
    assert(texGroupIndex <= UINT32_MAX);

    uint32_t layerBits = (uint32_t)drawLayer;
    uint32_t texBits = (uint32_t)texGroupIndex;

    return ((uint64_t)layerBits << 32) | (uint64_t)texBits;
}