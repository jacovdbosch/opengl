#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "buffer.h"
#include "stb_image.h"
#include <math.h>

const int WIDTH = 800;
const int HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


struct GameState {
    GLFWwindow* window;
    bool running = true;
};

GameState* createGameState() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn Opengl", NULL, NULL);
    
    if (window == NULL) {
        std::cout << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    
    framebufferSizeCallback(window, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    return new GameState { window };
}

void processInput(GameState* state) {
    if (glfwWindowShouldClose(state->window)) {
        state->running = false;
        
        return;
    }
    
    if (glfwGetKey(state->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        state->running = false;
    }
}

void deleteGameState(GameState* state) {
    glfwTerminate();
    delete state;
}

int main() {
    GameState *state = createGameState();
    
    if (state == nullptr)
        return -1;

    Shader *shader = createShader("E:\\cpp\\opengl\\src\\vertex.glsl",
                                  "E:\\cpp\\opengl\\src\\fragment.glsl");
    
    // Create a Vertex Array Object. This will be the  "container" used for all
    // the data that is bound to the shader. This has the advantage that we only
    // have to rebind the vao when we want to draw something that is given to the
    // gpu.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Generate the vertices needed for a rectangle.
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
    
    unsigned int VBO = createFloatBuffer(vertices, sizeof(vertices));
    assignVec3AttributeLayout(0, 8, 0);
    assignVec3AttributeLayout(1, 8, 3); 
    assignVec2AttributeLayout(2, 8, 6); 
    
    // Since we want to draw multiple triangles we will use a element buffer object. This 
    // will store the indices we will use to draw triangles. This will save up a lot of 
    // space because we do not have to store multiple of the same vec3 positions in 
    // memory (and send it to the gpu).
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    stbi_set_flip_vertically_on_load(true);
    
    // Texture creation
    unsigned int containerTexture = createTexture("E:\\cpp\\opengl\\src\\container.jpg", TEXTURE_RGB);
    unsigned int awesomeFaceTexture = createTexture("E:\\cpp\\opengl\\src\\awesome_face.png", TEXTURE_RGBA);
    
    if (containerTexture == 0 || awesomeFaceTexture == 0) {
        std::cout << "[ERROR]: Failed to load the texture" << std::endl;
    }
    
    useShader(shader);
    setShaderInt(shader, "texture1", 0);
    setShaderInt(shader, "texture2", 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, containerTexture);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, awesomeFaceTexture);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Render triangles as lines
    // only.
    
    while (state->running) {
        processInput(state);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        useShader(shader);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, containerTexture);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, awesomeFaceTexture);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(state->window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    deleteShader(shader);
    deleteGameState(state);
    
    return 0;
}