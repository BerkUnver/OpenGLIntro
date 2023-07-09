#pragma once
#include <GL/gl.h>

typedef struct Level {
    int width;
    int height;
    bool *data;

    Texture cube_texture;
    GLuint cube_vbo;
    GLuint cube_vao;
    GLuint cube_ebo;
} Level;

Level level_generate();
