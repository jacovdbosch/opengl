#include "opengl.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

internal char* load_file_as_string(const char* filePath) {
    FILE* fp = fopen(filePath, "rb");

    if (!fp) return NULL;

    fseek(fp, 0L, SEEK_END);
    long lSize = ftell(fp);
    rewind(fp);

    char* buffer = (char*)calloc(1, lSize+1);

    if (!buffer) {
        fclose(fp); 

        return NULL;
    }

    if (1!=fread(buffer, lSize, 1, fp)) {
        fclose(fp);
        free(buffer);

        return NULL;
    }

    fclose(fp);
    return buffer;
}

internal unsigned int compile_shader(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        error_log("Error compiling shader");
        error_log(infoLog);
    }

    return shader;
}

internal unsigned int create_shader_program(const char* vertexFilePath, const char* fragmentFilePath) {
    char* vertexSource = load_file_as_string(vertexFilePath);

    if (vertexSource == nullptr) {
        error_log("Error getting the vertex shader source");
        return -1;
    }

    char* fragmentSource = load_file_as_string(fragmentFilePath);

    if (fragmentSource == nullptr) {
        error_log("Error getting the fragment shader source");
        free(vertexSource);
        return -1;
    }

    unsigned int vertexShader = compile_shader(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compile_shader(fragmentSource, GL_FRAGMENT_SHADER);

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

        error_log("Error linking the shaders to the program");
        error_log(infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexSource);
    free(fragmentSource);

    return shaderProgram;
}

internal GameState* get_game_state(ApplicationState* state) {
    return (GameState*)state->gameMemory.memory;
}

internal unsigned int create_float_buffer(unsigned int size, void* data) {
    unsigned int bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId); 
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return bufferId;
}

internal void set_attribute_float_layout(unsigned int shader, const char* name, int size, int stride, int offset) {
    int attributePosition = glGetAttribLocation(shader, name);
    glVertexAttribPointer(attributePosition, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(attributePosition);
}

void on_viewport_resize(int width, int height) {
    glViewport(0, 0, width, height);
}

void on_game_startup(ApplicationState* appState) {
    unsigned int shaderProgram = create_shader_program(
            "E:\\cpp\\opengl\\src\\vertex.glsl",
            "E:\\cpp\\opengl\\src\\fragment.glsl");


    if (shaderProgram == 0) {
        glfwSetWindowShouldClose(appState->window, GLFW_TRUE);

        return;
    }

    GameState* state = get_game_state(appState);

    state->shader = shaderProgram;

    glUseProgram(state->shader);

    // Setup the VAO
    glGenVertexArrays(1, &state->VAO);
    glBindVertexArray(state->VAO);

    // Setup the buffers.
    float data[] = {
         0.0f,  0.5f, 1.0f, 0.0f, 0.0f, 
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    };
    state->VBO = create_float_buffer(sizeof(data), data);
    set_attribute_float_layout(state->shader, "position", 2, 5, 0);
    set_attribute_float_layout(state->shader, "color", 3, 5, 2);

    // Unbind so no data can be accidentally written to these buffers.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void on_game_destroy(ApplicationState* appState) {
    GameState* state = get_game_state(appState);

    glDeleteBuffers(1, &state->VBO);
    glDeleteVertexArrays(1, &state->VAO);
    glDeleteProgram(state->shader);
}

void on_game_update(ApplicationState* appState) {
    if (glfwGetKey(appState->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(appState->window, GLFW_TRUE);

        return;
    }

    // This was used to set the uniform ourColor to a changing green color value.
    /* GameState* state = get_game_state(appState); */

    /* float timeValue = glfwGetTime(); */
    /* float greenValue = (sin(timeValue) / 2.0f) + 0.5f; */

    /* int ourColorPos = glGetUniformLocation(state->shader, "ourColor"); */
    /* glUniform3f(ourColorPos, 1.0f, greenValue, 0.3f); */


}

void on_game_render(ApplicationState* appState) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GameState* state = get_game_state(appState);

    glBindVertexArray(state->VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
