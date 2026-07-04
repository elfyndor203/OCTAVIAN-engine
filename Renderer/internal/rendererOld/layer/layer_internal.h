#pragma once
#include "renderer/types_internal.h"

#include "cOCT_EngineStructure.h"
#include "Math.h"
#include <glad/glad.h>
#include <stdbool.h>

enum iOCT_attributes {
	iOCT_attrib_position = 0,
	iOCT_attrib_vertUV,
	iOCT_attrib_transformCol0,
	iOCT_attrib_transformCol1,
	iOCT_attrib_transformCol2,
	iOCT_attrib_color,
	iOCT_attrib_texUV,
	iOCT_attrib_dimensions
};

struct iOCT_layer {
	OCT_ID layerID;

	GLuint spriteVAO;

	GLuint spriteBuffer;
	cOCT_pool spriteDataPool;

	OCT_handle spriteAtlasHandle;
	GLuint spriteAtlas;

	bool dynamic;
};

iOCT_layer* iOCT_layer_get(OCT_ID layerID);

OCT_ID iOCT_layer_open(bool dynamic, OCT_handle texAtlasHandle);
void iOCT_layer_close(iOCT_layer* context);
//void iOCT_layer_draw(iOCT_layer* layer);
void iOCT_layer_drawAll();
