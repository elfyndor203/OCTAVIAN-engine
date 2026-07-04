// #include "texture_int.h"
//
// #include <glad/glad.h>
// #include <assert.h>
//
// GLuint iOCT_texture2D_load(unsigned char* pixels, int width, int height, OCT_handle resourceHandle) {
// 	GLuint texture;
// 	glGenTextures(1, &texture);
// 	glBindTexture(GL_TEXTURE_2D, texture);
// 	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
//
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
// 	assert(resourceHandle.objectID < iOCT_RENModule_instance.textureCapacity);
// 	iOCT_RENModule_instance.textureList[resourceHandle.objectID] = texture;
//
// 	cOCT_message RESMsg = {
// 		.messageType = cOCT_MSG_TEXTURE_DONE,
// 		.texture_done = {
// 			.texHandle = resourceHandle
// 		}
// 	};
// 	return texture;
// }
//
// GLuint iOCT_texture2D_get(OCT_handle resourceHandle) {
// 	assert(resourceHandle.objectID < iOCT_RENModule_instance.textureCapacity);
// 	return iOCT_RENModule_instance.textureList[resourceHandle.objectID];
// }