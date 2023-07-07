#include <stdio.h>
#include <windows.h>
#include "glad/glad.h"
#include "glfw/glfw3.h"

#define WINDOW_X 800
#define WINDOW_Y 800

#define SHADER_LOG_SIZE 1024

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

const char *shader_vertex_source = R""(
#version 330 core
layout (location = 0) in vec3 pos;

void main() {
    gl_Position = vec4(pos, 1.0);
}
)"";

const char *shader_fragment_source = R""(
#version 330 core
out vec4 fragment_color;

void main() {
    fragment_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
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
    }

    glDeleteShader(shader_vertex);
    glDeleteShader(shader_fragment);

    GLuint vao;
    GLuint vbo;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
