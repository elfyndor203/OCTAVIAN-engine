#include "layer_internal.h"
#include "renderer/types_internal.h"

#include "cOCT_EngineStructure.h"
#include "_ECS_Output/_ECS_include.h"
#include <glad/glad.h>
#include <stdio.h>
#include <assert.h>

#include "module/RENModule_internal.h"
#include "renderer/spriteData/spriteData_internal.h"
#include "renderer/texture/texture_internal.h"


iOCT_layer* iOCT_layer_get(OCT_ID layerID) {
	OCT_index index = cOCT_IDMap_getIndex(&iOCT_RENModule_instance.layerMap, layerID);
	return (iOCT_layer*)cOCT_pool_access(&iOCT_RENModule_instance.layerPool, index);
}

/// <summary>
/// Creates a new visual layer. All sprites on each layer use the same texture atlas. Any loaded images must be uploaded using OCT_RENModule_flush() before layers are created or a frame will be skipped.
/// </summary>
/// <param name="dynamic"></param>
/// <param name="texAtlas"></param>
/// <returns></returns>
OCT_handle OCT_layer_open(bool dynamic, OCT_handle texAtlas) {
	OCT_handle newLayer = {
		.containerID = OCT_subsystem_renderer,
		.objectID = iOCT_layer_open(dynamic, texAtlas),
		.subsystem = OCT_subsystem_renderer,
		.type = OCT_handle_layer
	};
	return newLayer;
}
OCT_ID iOCT_layer_open(bool dynamic, OCT_handle texAtlasHandle) {
	OCT_index newIndex;
	OCT_ID newID;
	iOCT_layer* newLayer;

	// register layer
	newLayer = (iOCT_layer*)cOCT_pool_addEntry(&iOCT_RENModule_instance.layerPool, &newIndex);
	newID = cOCT_IDMap_register(&iOCT_RENModule_instance.layerMap, newIndex);

	// set defaults, init pool/map
	newLayer->layerID = newID;
	newLayer->spriteDataPool = cOCT_pool_init(newID, cOCT_POOLSIZE_DEFAULT, sizeof(iOCT_spriteData));
	newLayer->dynamic = dynamic;
	newLayer->spriteAtlasHandle = texAtlasHandle;
	newLayer->spriteAtlas = iOCT_texture2D_get(texAtlasHandle);
	
	// VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, iOCT_RENModule_instance.spriteVBO);
	glVertexAttribPointer(iOCT_attrib_position, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(iOCT_attrib_position);
	glVertexAttribPointer(iOCT_attrib_vertUV, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(iOCT_attrib_vertUV);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iOCT_RENModule_instance.spriteEBO);

	// spritebuffer
	glGenBuffers(1, &newLayer->spriteBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, newLayer->spriteBuffer);
	glBufferData(GL_ARRAY_BUFFER, cOCT_POOLSIZE_DEFAULT * sizeof(iOCT_spriteData), NULL, GL_DYNAMIC_DRAW);	// initial size

		// matrix
	glVertexAttribPointer(iOCT_attrib_transformCol0, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteData), (void*)offsetof(iOCT_spriteData, transform.c0r0));
	glEnableVertexAttribArray(iOCT_attrib_transformCol0);
	glVertexAttribDivisor(iOCT_attrib_transformCol0, 1);

	glVertexAttribPointer(iOCT_attrib_transformCol1, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteData), (void*)offsetof(iOCT_spriteData, transform.c1r0));
	glEnableVertexAttribArray(iOCT_attrib_transformCol1);
	glVertexAttribDivisor(iOCT_attrib_transformCol1, 1);

	glVertexAttribPointer(iOCT_attrib_transformCol2, 3, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteData), (void*)offsetof(iOCT_spriteData, transform.c2r0));
	glEnableVertexAttribArray(iOCT_attrib_transformCol2);
	glVertexAttribDivisor(iOCT_attrib_transformCol2, 1);

		// color
	glVertexAttribPointer(iOCT_attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteData), (void*)offsetof(iOCT_spriteData, color));
	glEnableVertexAttribArray(iOCT_attrib_color);
	glVertexAttribDivisor(iOCT_attrib_color, 1);

		// uv
	glVertexAttribPointer(iOCT_attrib_texUV, 4, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteData), (void*)offsetof(iOCT_spriteData, uvRect));
	glEnableVertexAttribArray(iOCT_attrib_texUV);
	glVertexAttribDivisor(iOCT_attrib_texUV, 1);

		// dimensions
	glVertexAttribPointer(iOCT_attrib_dimensions, 2, GL_FLOAT, GL_FALSE, sizeof(iOCT_spriteData), (void*)offsetof(iOCT_spriteData, dimensions));
	glEnableVertexAttribArray(iOCT_attrib_dimensions);
	glVertexAttribDivisor(iOCT_attrib_dimensions, 1);

	newLayer->spriteVAO = VAO;

	return newID;
}

void iOCT_layer_close(iOCT_layer* layer) {
	cOCT_pool_free(&layer->spriteDataPool);
}

void iOCT_layer_drawAll() {
	OCT_counter contextCount = _OCT_entityContext_getCount();
	OCT_counter spriteCount;
	_OCT_snapshot_sprite ECSSpriteData;
	iOCT_layer* layer;
	iOCT_spriteData* slot;
	OCT_mat3 cameraMatrix = OCT_mat3_inverse(_OCT_camera2D_getActiveMatrix());

	OCT_index poolDummy;
	for (int l = 0; l < iOCT_RENModule_instance.layerPool.count; l++) {			// Pass 1: wipe all pools
		layer = &((iOCT_layer*)iOCT_RENModule_instance.layerPool.array)[l];
		layer->spriteDataPool.count = 0;
	}

	for (int context = 0; context < contextCount; context++) {					// Pass 2: load all layers with sprite data
		spriteCount = _OCT_sprite2D_getCount(context);
		for (int sprite = 0; sprite < spriteCount; sprite++) {
			ECSSpriteData = _OCT_sprite2D_getSnapshot(sprite, context);

			if (!ECSSpriteData.visible) {
				continue;
			}

			assert(ECSSpriteData.layer.type == OCT_handle_layer);
			layer = iOCT_layer_get(ECSSpriteData.layer.objectID);
			slot = cOCT_pool_addEntry(&layer->spriteDataPool, &poolDummy);
			slot->color = ECSSpriteData.color;
			slot->dimensions = ECSSpriteData.dimensions;
			slot->transform = ECSSpriteData.transform;
			slot->uvRect = ECSSpriteData.uv;
		}
	}

	glUseProgram(iOCT_RENModule_instance.spriteShader);
	glUniformMatrix3fv(iOCT_RENModule_instance.cameraProjUniform, 1, GL_FALSE, (float*)&cameraMatrix);	// upload changing uniforms
	for (int l = 0; l < iOCT_RENModule_instance.layerPool.count; l++) {			// Pass 3: draw everything
		layer = &((iOCT_layer*)iOCT_RENModule_instance.layerPool.array)[l];

		if (layer->spriteDataPool.count == 0) {
			continue;
		}

		// VAO
		glBindVertexArray(layer->spriteVAO);
		// Shaders
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, layer->spriteAtlas);
		// Buffer
		glBindBuffer(GL_ARRAY_BUFFER, layer->spriteBuffer);
		glBufferData(GL_ARRAY_BUFFER, layer->spriteDataPool.count * sizeof(iOCT_spriteData), layer->spriteDataPool.array, GL_DYNAMIC_DRAW);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)layer->spriteDataPool.count);
	}
}


