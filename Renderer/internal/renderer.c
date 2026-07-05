#include "renderer_int.h"
#include "types_int.h"

#include <stdio.h>
#include <glad/glad.h>

#include "textureGroup_int.h"
#include "sprite2D_int.h"
#include "shader/shader/shader_int.h"

#define iOCT_SPRITES_INITIAL_CAPACITY 16
#define iOCT_SPRITES_EXPANSION_FACTOR 2
#define iOCT_QUAD_INDEX_CT 6

typedef enum iOCT_spriteAttributes {
    attrib_quadXY,
    attrib_quadUV,
    attrib_uv,
    attrib_color,
    attrib_dimensions,
    attrib_arrayLayer,
    attrib_transformCol0,
    attrib_transformCol1,
    attrib_transformCol2,
} iOCT_spriteAttributes;
iOCT_renderer iOCT_renderer_inst = { 0 };

// basic quad shape
typedef struct quadVertex {
    float x;
    float y;
    float u;
    float v;
} quadVertex;
static quadVertex spriteVertices[] = {
    {-0.5f,  0.5f,     0.0f, 1.0f}, // top left
    { 0.5f,  0.5f,     1.0f, 1.0f}, // top right
     {0.5f, -0.5f,     1.0f, 0.0f}, // bottom right
    {-0.5f, -0.5f,     0.0f, 0.0f}  // bottom left
};
static unsigned int spriteIndices[] = {
    0, 1, 2,
    2, 3, 0,
};

static void iOCT_setInstanceOffset(GLuint VAO, GLuint VBO, size_t byteOffset);
static void iOCT_setupNewSpriteVAO(GLuint VAO, GLuint quadVBO,  GLuint quadEBO, GLuint spriteVBO);

void system_init_RENDERER() {
    OCT_ID systemID = iOCT_renderer_inst.systemDescription.systemID_reg;
    iOCT_renderer_inst.textureGroupPool = eOCT_pool_init(systemID, eOCT_POOL_SIZE_DEFAULT, sizeof(iOCT_textureGroup));
    iOCT_renderer_inst.textureGroupMap = eOCT_IDMap_init(systemID, eOCT_POOL_SIZE_DEFAULT);
    iOCT_renderer_inst.spriteFullDataBuffer = eOCT_pool_init(systemID, eOCT_POOL_SIZE_DEFAULT,sizeof(iOCT_spriteFullData));

    GLuint spriteVAO;
    glGenVertexArrays(1, &spriteVAO);
    glBindVertexArray(spriteVAO);

#pragma region basic quad layout
    GLuint quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(spriteVertices), spriteVertices, GL_STATIC_DRAW);
    // quad xy
    glVertexAttribPointer(attrib_quadXY, 2, GL_FLOAT, GL_FALSE, sizeof(quadVertex), (void*)0);
    glEnableVertexAttribArray(0);
    // quad uv
    glVertexAttribPointer(attrib_quadUV, 2, GL_FLOAT, GL_FALSE, sizeof(quadVertex), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint quadEBO;
    glGenBuffers(1, &quadEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(spriteIndices), spriteIndices, GL_STATIC_DRAW);
#pragma endregion

#pragma region uploaded data layout
    GLuint spriteDataVBO;
    glGenBuffers(1, &spriteDataVBO);
    glBindBuffer(GL_ARRAY_BUFFER, spriteDataVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(iOCT_spriteFullData) * iOCT_SPRITES_INITIAL_CAPACITY, iOCT_renderer_inst.spriteFullDataBuffer.array, GL_DYNAMIC_DRAW);

    // glVertexAttribPointer(attrib_uv, 4, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, uv))); //uv
    // glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, color)));
    // glVertexAttribPointer(attrib_dimensions, 2, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, dimensions)));
    // glVertexAttribPointer(attrib_arrayLayer, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, texArrayLayer)));
    // glVertexAttribPointer(attrib_transformCol0, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c0r0)));
    // glVertexAttribPointer(attrib_transformCol1, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c1r0)));
    // glVertexAttribPointer(attrib_transformCol2, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c2r0)));
    iOCT_setupNewSpriteVAO(spriteVAO, quadVBO, quadEBO, spriteDataVBO);


#pragma endregion

#pragma region shaders
    GLuint spriteShader = iOCT_shader_createProgram("OCTAVIANEngine/Renderer/internal/shader/sprite/sprite.vert", "OCTAVIANEngine/Renderer/internal/shader/sprite/sprite.frag");
    glUseProgram(spriteShader);
    glUniform1i(glGetUniformLocation(spriteShader, "spriteTexArray"), 0);
    glUseProgram(0);
#pragma endregion
    glBindVertexArray(0);
    iOCT_renderer_inst.spriteVAO = spriteVAO;
    iOCT_renderer_inst.quadEBO = quadEBO;
    iOCT_renderer_inst.quadVBO = quadVBO;
    iOCT_renderer_inst.spriteDataVBO = spriteDataVBO;
    iOCT_renderer_inst.spriteDataVBOCapacity = iOCT_SPRITES_INITIAL_CAPACITY;
    iOCT_renderer_inst.spriteShaderProgram = spriteShader;
}

void iOCT_renderer_uploadAll(OCT_handle contextHandle) {
    eOCT_pool_clear(&iOCT_renderer_inst.spriteFullDataBuffer);

    // Source
    eOCT_pool* spritePool = eOCT_getComponentPool(contextHandle, iOCT_renderer_inst.sprite2DCache);
    iOCT_sprite2D* spriteArray = (iOCT_sprite2D*)spritePool->array;
    // Buffer
    eOCT_pool* spriteBufferPool = &iOCT_renderer_inst.spriteFullDataBuffer;
    if (spritePool->count > spriteBufferPool->capacity) {
        eOCT_pool_expand(spriteBufferPool, spritePool->count);
    }

    for (OCT_index spriteCtr = 0; spriteCtr < spritePool->count; spriteCtr++) {
        iOCT_sprite2D sprite = spriteArray[spriteCtr];
        iOCT_textureGroup texGroup = *(iOCT_textureGroup*)eOCT_getByID(&iOCT_renderer_inst.textureGroupMap, &iOCT_renderer_inst.textureGroupPool, sprite.texGroupID);
        OCT_index texArrayLayer = eOCT_IDMap_getIndex(&texGroup.textureMap, sprite.texID);

        iOCT_spriteData spriteData = spriteArray[spriteCtr].spriteData;
        OCT_mat3* entityTransformPtr = (OCT_mat3*)eOCT_entity_getField(sprite.entity, iOCT_renderer_inst.transform2DCache);
        OCT_mat3 entityTransform;
        if (entityTransformPtr == NULL) {
            entityTransform = OCT_mat3_identity;
            //printf("Has no transform\n");
        }
        else {
            entityTransform = *entityTransformPtr;
        }
        OCT_mat3 finalTransform = OCT_mat3_mul(entityTransform, sprite.spriteTransform);

        spriteData.texArrayLayer = (GLuint)texArrayLayer;

        iOCT_spriteFullData* fullData = (iOCT_spriteFullData*)eOCT_pool_addEntry(spriteBufferPool, NULL);
        fullData->spriteData = spriteData;
        fullData->transform = finalTransform;
    }

    glBindBuffer(GL_ARRAY_BUFFER, iOCT_renderer_inst.spriteDataVBO);
    if (spriteBufferPool->count > iOCT_renderer_inst.spriteDataVBOCapacity) {
        glBufferData(GL_ARRAY_BUFFER, iOCT_SPRITES_EXPANSION_FACTOR * sizeof(iOCT_spriteFullData) * iOCT_renderer_inst.spriteDataVBOCapacity, iOCT_renderer_inst.spriteFullDataBuffer.array, GL_DYNAMIC_DRAW);
        iOCT_renderer_inst.spriteDataVBOCapacity *= iOCT_SPRITES_EXPANSION_FACTOR;
    }
    else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, spriteBufferPool->count * spriteBufferPool->elementSize, spriteBufferPool->array);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void iOCT_renderer_drawAll(OCT_handle contextHandle) {
    eOCT_pool* spritePool = eOCT_getComponentPool(contextHandle, iOCT_renderer_inst.sprite2DCache);
    iOCT_sprite2D* spriteArray = (iOCT_sprite2D*)spritePool->array;

    if (spritePool->count == 0) {
        return;
    }

    glBindVertexArray(iOCT_renderer_inst.spriteVAO);
    glUseProgram(iOCT_renderer_inst.spriteShaderProgram);

    OCT_index spriteCtr = 0;
    GLsizei drawnCt = 0;

    // iterate through all sprites
    while (spriteCtr < spritePool->count) {
        // note the current batch
        OCT_index currentLayer = spriteArray[spriteCtr].drawLayer;
        OCT_ID currentTexGroup = spriteArray[spriteCtr].texGroupID;
        GLsizei batchCt = 0;

        // incremenet until a different batch is hit
        while (spriteCtr < spritePool->count &&
               spriteArray[spriteCtr].drawLayer == currentLayer &&
               spriteArray[spriteCtr].texGroupID == currentTexGroup) {
            batchCt++;
            spriteCtr++;
               }

        iOCT_textureGroup texGroup = *(iOCT_textureGroup*)eOCT_getByID(&iOCT_renderer_inst.textureGroupMap, &iOCT_renderer_inst.textureGroupPool, currentTexGroup);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texGroup.glTexArray);

        // create the offset for drawElementsInstanced
        size_t drawnByteOffset = drawnCt * iOCT_renderer_inst.spriteFullDataBuffer.elementSize;
        iOCT_setInstanceOffset(iOCT_renderer_inst.spriteVAO, iOCT_renderer_inst.spriteDataVBO, drawnByteOffset);
        glDrawElementsInstanced(GL_TRIANGLES, iOCT_QUAD_INDEX_CT, GL_UNSIGNED_INT, (void*)0, batchCt); // void0 is not buffer offset

        drawnCt += batchCt;
    }
}

void eOCT_RENDERER_update(OCT_handle contextHandle) {
    iOCT_renderer_uploadAll(contextHandle);
    iOCT_renderer_drawAll(contextHandle);
}

static void iOCT_setInstanceOffset(GLuint VAO, GLuint VBO, size_t byteOffset) {
    size_t spriteDataBase = offsetof(iOCT_spriteFullData, spriteData);
    size_t transformBase = offsetof(iOCT_spriteFullData, transform);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(attrib_uv, 4, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, uv) + byteOffset)); //uv
    glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, color) + byteOffset));
    glVertexAttribPointer(attrib_dimensions, 2, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, dimensions) + byteOffset));
    glVertexAttribPointer(attrib_arrayLayer, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(spriteDataBase + offsetof(iOCT_spriteData, texArrayLayer) + byteOffset));
    glVertexAttribPointer(attrib_transformCol0, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c0r0) + byteOffset));
    glVertexAttribPointer(attrib_transformCol1, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c1r0) + byteOffset));
    glVertexAttribPointer(attrib_transformCol2, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteFullData), (void*)(transformBase + offsetof(OCT_mat3, c2r0) + byteOffset));
}

static void iOCT_setupNewSpriteVAO(GLuint VAO, GLuint quadVBO,  GLuint quadEBO, GLuint spriteVBO) {
    glBindVertexArray(VAO);

    // quad VBO
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glVertexAttribPointer(attrib_quadXY, 2, GL_FLOAT, GL_FALSE, sizeof(quadVertex), (void*)0);
    glEnableVertexAttribArray(0);
        // quad uv
    glVertexAttribPointer(attrib_quadUV, 2, GL_FLOAT, GL_FALSE, sizeof(quadVertex), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // quad EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);

    // spriteVBO
    glBindBuffer(GL_ARRAY_BUFFER, spriteVBO);
    iOCT_setInstanceOffset(VAO, spriteVBO, 0);
    glEnableVertexAttribArray(attrib_uv);
    glEnableVertexAttribArray(attrib_color);
    glEnableVertexAttribArray(attrib_dimensions);
    glEnableVertexAttribArray(attrib_arrayLayer);
    glEnableVertexAttribArray(attrib_transformCol0);
    glEnableVertexAttribArray(attrib_transformCol1);
    glEnableVertexAttribArray(attrib_transformCol2);
    glVertexAttribDivisor(attrib_uv, 1);
    glVertexAttribDivisor(attrib_color, 1);
    glVertexAttribDivisor(attrib_dimensions, 1);
    glVertexAttribDivisor(attrib_arrayLayer, 1);
    glVertexAttribDivisor(attrib_transformCol0, 1);
    glVertexAttribDivisor(attrib_transformCol1, 1);
    glVertexAttribDivisor(attrib_transformCol2, 1);
}

void OCT_renderToNewWindow(OCT_handle window) {
    GLuint windowVAO = (GLuint)*(uint64_t*)eOCT_getGlobalDataField(iOCT_renderer_inst.windowVAOCache, window.objectID);

    iOCT_setupNewSpriteVAO(windowVAO, iOCT_renderer_inst.quadVBO, iOCT_renderer_inst.quadEBO, iOCT_renderer_inst.spriteDataVBO);
}
