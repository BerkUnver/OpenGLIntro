#include <windows.h>
#include <glad/glad.h>
#include "stb/stb_image.h"
#include "draw.h"

bool texture_load(const char *path, Texture *texture) {
    void *texture_raw = stbi_load(path, &texture->width, &texture->height, &texture->color_channels, STBI_rgb_alpha);
    if (!texture_raw) return false;
    
    glGenTextures(1, &texture->handle);
    glBindTexture(GL_TEXTURE_2D, texture->handle);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_raw);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(texture_raw);
    return true;
}

void texture_free(Texture *texture) {
    glDeleteTextures(1, &texture->handle);
}

void texture_use(Texture *texture, GLenum unit) {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, texture->handle);
}
