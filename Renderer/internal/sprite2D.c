#include "sprite2D_int.h"
#include "types_int.h"

#include "OCT_Core_eng.h"
#include "stdio.h"

#include "renderer_int.h"
#include "textureGroup_int.h"

void OCT_sprite2D_attach(OCT_handle entity, OCT_handle texture, OCT_vec4 uv, OCT_vec4 color, OCT_index drawLayer) {
    iOCT_sprite2D* newSprite = eOCT_entity_attachComponent(entity, iOCT_renderer_inst.sprite2DCache);
    newSprite->entity = entity;
    newSprite->texGroupID = texture.containerID;
    newSprite->spriteData.uv = uv;
    newSprite->spriteData.color = color;
    newSprite->spriteData.transform = OCT_mat3_identity;

    printf("Attached sprite2D to entity %zu", entity.objectID);

    iOCT_textureGroup* texGroup = (iOCT_textureGroup*)eOCT_getByID(&iOCT_renderer_inst.textureGroupMap, &iOCT_renderer_inst.textureGroupPool, texture.containerID);
}