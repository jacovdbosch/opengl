#include "renderer.h"
#include "opengl.h"
#include "platform.h"

struct Renderer {
    unsigned int program;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};

global Renderer renderer;

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
    char* vertexSource = load_file(vertexFilePath);

    if (vertexSource == nullptr) {
        error_log("Error getting the vertex shader source");
        return -1;
    }

    char* fragmentSource = load_file(fragmentFilePath);

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

internal void set_attribute_float_layout(unsigned int shader, const char* name, int size, int stride, int offset) {
    int attributePosition = glGetAttribLocation(shader, name);
    glVertexAttribPointer(attributePosition, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(attributePosition);
}

internal void setVertexBufferData(int size, void* data) {
    glBindBuffer(GL_ARRAY_BUFFER, renderer.VBO);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

internal void setElementBufferData(int size, void* data) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int initialize_renderer() {
    unsigned int shaderProgram = create_shader_program(
            "E:\\cpp\\opengl\\assets\\vertex.glsl",
            "E:\\cpp\\opengl\\assets\\fragment.glsl");

    if (shaderProgram == 0) return -1;

    renderer.program = shaderProgram;

    glUseProgram(renderer.program);

    // Setup the VAO
    glGenVertexArrays(1, &renderer.VAO);
    glBindVertexArray(renderer.VAO);

    // Setup the VBO and attrib pointers
    glGenBuffers(1, &renderer.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.VBO);
    set_attribute_float_layout(renderer.program, "position", 2, 5, 0);
    set_attribute_float_layout(renderer.program, "color", 3, 5, 2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Setup the EBO
    glGenBuffers(1, &renderer.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return 0;
}

void shutdown_renderer() {
    glDeleteBuffers(1, &renderer.VBO);
    glDeleteVertexArrays(1, &renderer.VAO);
    glDeleteProgram(renderer.program);
}

void clear_buffer(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void submit_buffer() {
    glBindVertexArray(renderer.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void draw_rectangle(float x, float y, float width, float height) {
    glBindVertexArray(renderer.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
