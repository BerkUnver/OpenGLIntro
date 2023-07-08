#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define WINDOW_X 800
#define WINDOW_Y 800

#define SHADER_LOG_SIZE 1024

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

#define VERTEX_IN_SIZE 8
float vertices[] = {
    0.5f, 0.5f, 0.0f,       1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.5f, -0.5f, 0.0f,      1.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,      0.0f, 0.0f, 1.0f,   0.0f, 1.0f
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

const char *shader_vertex_source = R""(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texture_coords;

out vec3 fragment_color;
out vec2 fragment_texture_coords;

void main() {
    fragment_color = vertex_color;
    fragment_texture_coords = vertex_texture_coords;
    gl_Position = vec4(pos, 1.0);
}
)"";

const char *shader_fragment_source = R""(
#version 330 core

uniform sampler2D texture_0;

in vec3 fragment_color;
in vec2 fragment_texture_coords;

out vec4 color;

void main() {
    color = texture(texture_0, fragment_texture_coords) * vec4(fragment_color, 1.0f);
}
)"";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_X, WINDOW_Y, "Stealth", NULL, NULL);
    if (!window) {
        printf("Failed to open GLFW window.\n");
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD and fetch OpenGL extensions.\n");
        return -1;
    }

    glViewport(0, 0, WINDOW_X, WINDOW_Y);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);    
    
    // Load shader
    char shader_log[SHADER_LOG_SIZE];
    
    GLuint shader_vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_vertex, 1, &shader_vertex_source, NULL);
    glCompileShader(shader_vertex);

    int shader_vertex_success;
    glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &shader_vertex_success);
    if (!shader_vertex_success) {
        glGetShaderInfoLog(shader_vertex, SHADER_LOG_SIZE, NULL, shader_log);
        printf("Failed to compile vertex shader! Error:\n%s", shader_log);
    }
    
    GLuint shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_fragment, 1, &shader_fragment_source, NULL);
    glCompileShader(shader_fragment);

    int shader_fragment_success;
    glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &shader_fragment_success);
    if (!shader_fragment_success) {
        glGetShaderInfoLog(shader_fragment, SHADER_LOG_SIZE, NULL, shader_log);
        printf("Failed to compile fragment shader! Error:\n%s", shader_log);
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
        return -1;
    }

    glDeleteShader(shader_vertex);
    glDeleteShader(shader_fragment);
    
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "texture_0"), 0);
    glUseProgram(0);

    // Bind vertex data
    GLuint vao;
    GLuint vbo;
    GLuint ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_IN_SIZE * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_IN_SIZE * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERTEX_IN_SIZE * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // Bind texture 
    stbi_set_flip_vertically_on_load(true);
    
    int texture_width;
    int texture_height;
    int texture_color_channels;
    
    void *texture_raw = stbi_load("../data/bricks.png", &texture_width, &texture_height, &texture_color_channels, STBI_rgb_alpha);
    if (!texture_raw) {
        printf("Failed to open texture. Error: %s\n", stbi_failure_reason());
        return -1;
    }
    
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_raw);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(texture_raw);
    
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(*indices), GL_UNSIGNED_INT, 0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader);

    glfwTerminate();
    
    return 0;
}
