#include "shader_int.h"

#include "OCT_Core_eng.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

GLuint iOCT_shader_load(char* path, int type) {
	FILE* file;
	long size;
	char* src;
	size_t read;
	GLuint shader;
	if (type == iOCT_SHADER_VERT) {
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}
	
	file = fopen(path, "rb");
	if (!file) {
		OCT_ERROR_LOG(OCT_EXIT_FAILED_TO_OPEN_FILE, "Could not open shader file");
		return 0;
	}
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	src = malloc(size + 1);
	if (!src) {
		OCT_ERROR_LOG(OCT_EXIT_FAILED_TO_ALLOCATE, "Failed to allocate shader source");
		return 0;
	}
	read = fread(src, 1, size, file);
	src[read] = '\0';

	glShaderSource(shader, 1, (const char *const *)&src, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(shader, 512, NULL, log);
		printf("Shader error: %s\n", log);
	}

	printf("Shader loaded\n");
	free(src);
	return shader;
}

GLuint iOCT_shader_createProgram(char* vertPath, char* fragPath) {
	GLuint vertShader = iOCT_shader_load(vertPath, iOCT_SHADER_VERT);
	GLuint fragShader = iOCT_shader_load(fragPath, iOCT_SHADER_FRAG);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char log[512];
		glGetProgramInfoLog(program, 512, NULL, log);
		printf("Program link error: %s\n", log);
	}

	glUseProgram(program);

	printf("Shader program created\n");
	return program;
}