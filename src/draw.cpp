#include <windows.h>
#include <glad/glad.h>
#include "stb/stb_image.h"
#include "draw.h"

#define SHADER_LOG_SIZE 1024

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

bool shader_load(const char *vs, const char *fs, GLuint *shader_out) { 
    char shader_log[SHADER_LOG_SIZE];
    
    GLuint shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_vertex, 1, &vs, NULL);
    glCompileShader(shader_vertex);

    int shader_vertex_success;
    glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &shader_vertex_success);
    if (!shader_vertex_success) {
        glGetShaderInfoLog(shader_vertex, SHADER_LOG_SIZE, NULL, shader_log);
        printf("Failed to compile vertex shader at \"%s\".\nError:\n%s", vs, shader_log);
        return false;
    }
    
    GLuint shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_fragment, 1, &fs, NULL);
    glCompileShader(shader_fragment);

    int shader_fragment_success;
    glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &shader_fragment_success);
    if (!shader_fragment_success) {
        glGetShaderInfoLog(shader_fragment, SHADER_LOG_SIZE, NULL, shader_log);
        printf("Failed to compile fragment shader at \"%s\".\nError:\n%s", fs, shader_log);
        return false;
    }
    
    GLuint shader = glCreateProgram();
    glAttachShader(shader, shader_vertex);
    glAttachShader(shader, shader_fragment);
    glLinkProgram(shader);

    int shader_link_success;
    glGetProgramiv(shader, GL_LINK_STATUS, &shader_link_success);
    if (!shader_link_success) {
        glGetProgramInfoLog(shader, SHADER_LOG_SIZE, NULL, shader_log);
        printf("Failed to link shader! Error: \n%s", shader_log);
        return false;
    }

    glDeleteShader(shader_vertex);
    glDeleteShader(shader_fragment);

    *shader_out = shader;
    return true;
}
