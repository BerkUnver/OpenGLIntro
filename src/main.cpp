#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#undef near
#undef far

#include "glad/glad.h"
#include "glfw/glfw3.h"
#define RAYMATH_IMPLEMENTATION
#include "raylib/raymath.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define WINDOW_X 800
#define WINDOW_Y 800

#define SHADER_LOG_SIZE 1024
#define PI 3.14159265358979323846f
#define TAU (PI * 2.0f)
#define MATRIX_TRANSPOSED GL_TRUE // Whether the matrix type is stored as transposed compared to an OpenGL matrix

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

#define VERTEX_IN_SIZE 8
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f
};

#define MAP_X 9
#define MAP_Y 9
int map[] = {
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 1, 1, 1,
};

const char *shader_vertex_source = R""(
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texture_coords;

uniform mat4 transform;

out vec3 fragment_color;
out vec2 fragment_texture_coords;

void main() {
    fragment_color = vertex_color;
    fragment_texture_coords = vertex_texture_coords;
    gl_Position = transform * vec4(pos, 1.0);
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
    glEnable(GL_DEPTH_TEST);

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
    int shader_loc_texture_0 = glGetUniformLocation(shader, "texture_0");
    int shader_loc_transform = glGetUniformLocation(shader, "transform");
    glUniform1i(shader_loc_texture_0, 0);
    glUseProgram(0);

    // Bind vertex data
    GLuint vao;
    GLuint vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
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

    Vector3 view_offset = { 0.0f, 10.0f, 5.0f };
    Vector3 view_position = view_offset;
    double time_previous = glfwGetTime();
    
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break; 
        float time_current = glfwGetTime();
        float delta = (float) (time_current - time_previous);
        time_previous = time_current;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);

        int axis_x = (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
        int axis_y = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) - (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
        view_position.x += axis_x * delta;
        view_position.z -= axis_y * delta; // z axis is inverted

        Vector3 view_target = Vector3Subtract(view_position, view_offset); 
        Matrix view = MatrixLookAt(view_position, view_target, {0.0f, 1.0f, 0.0f});
        
        Matrix projection = MatrixPerspective(PI / 3.0f, (float) WINDOW_X / (float) WINDOW_Y, 0.1f, 100.0f); 
        
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        
       
        for (int x = 0; x < MAP_X; x++)
        for (int y = 0; y < MAP_Y; y++) {
            if (!map[y * MAP_X + x]) continue;
            float model_x = (float) ((-MAP_X + 1) / 2 + x);
            float model_z = (float) ((-MAP_Y + 1) / 2 + y); 
            Matrix model = MatrixTranslate(model_x, 0.0f, model_z);
            Matrix mvp = MatrixMultiply(MatrixMultiply(model, view), projection);
            glUniformMatrix4fv(shader_loc_transform, 1, MATRIX_TRANSPOSED, (float *) &mvp);
            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(*vertices));
        }
        
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader);

    glfwTerminate();
    
    return 0;
}
