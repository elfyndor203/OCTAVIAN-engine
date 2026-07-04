#pragma once

#include "OCT_Core_eng.h"
#include "glad/glad.h"

struct iOCT_textureAtlas {
    GLuint glHandle;
};

//__NOTE_ opengl texture arrays

GLuint iOCT_texture2D_load(unsigned char* pixels, int width, int height, OCT_handle resourceHandle);
GLuint iOCT_texture2D_get(OCT_handle resourceHandle);