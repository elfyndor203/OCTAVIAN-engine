#include "image_internal.h"
#include "resources/types_internal.h"

#include "cOCT_Communication.h"
#include "cOCT_EngineStructure.h"
#include <string.h>

#include "resources/resource/resource_internal.h"
#include "resources/resourceList/resourceList_internal.h"
#include "module/RESModule_internal.h"

#define iOCT_RGBA 4

OCT_handle OCT_image_load(const char* path) {
	OCT_handle handle = iOCT_image_load(path);
	return handle;
}
OCT_handle iOCT_image_load(const char* path) {
	stbi_set_flip_vertically_on_load(1);

	iOCT_resourceList* list = &iOCT_RESModule_instance.imageList;

	OCT_index newIndex;
	OCT_ID newID;
	iOCT_resource* newImage;

	newImage = cOCT_pool_addEntry(&list->pool, &newIndex);
	newID = cOCT_IDMap_register(&list->map, newIndex);
	
	newImage->listID = list->listID;
	strncpy(newImage->path, path, iOCT_RESOURCE_PATHNAME_MAX - 1);
	newImage->path[iOCT_RESOURCE_PATHNAME_MAX - 1] = '\0';
	newImage->resourceID = newID;
	newImage->type = iOCT_resourceImage;

	int width;
	int height;
	int channels;
	unsigned char* pixels = stbi_load(path, &width, &height, &channels, iOCT_RGBA);
	if (!pixels) {
		printf("Failed load\n");
		printf(stbi_failure_reason());
	}

	OCT_handle handle = {
		.subsystem = OCT_subsystem_resources,
		.containerID = list->listID,
		.objectID = newID,
		.type = OCT_handle_image
	};

	cOCT_message renderMSG = {
		.messageType = cOCT_MSG_TEXTURE_LOAD,
		.texture_load = {
			.pixels = pixels,
			.texHandle = handle,
			.width = width,
			.height = height
			}
	};

	printf("Loaded image %p\n", pixels);
	cOCT_message_push(OCT_subsystem_renderer, renderMSG, cOCT_INBOX);
	return handle;
}