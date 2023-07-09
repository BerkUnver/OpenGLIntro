#pragma once
#include <windows.h>
#include <glad/glad.h>

typedef struct Texture {
    GLuint handle;
    int width;
    int height;
    int color_channels;
} Texture;

bool texture_load(const char *path, Texture *texture);
void texture_free(Texture *texture);
void texture_use(Texture *texture, GLenum unit);

bool shader_load(const char *vs, const char *fs, GLuint *shader);
