#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader.h"
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
    GameState* state = createGameState();
    
    if (state == nullptr) return -1;
    
    Shader* shader = createShader("E:\\cpp\\opengl\\src\\vertex.glsl", 
                                  "E:\\cpp\\opengl\\src\\fragment.glsl");
    
    // Create a Vertex Array Object. This will be the  "container" used for all the data that is 
    // bound to the shader. This has the advantage that we only have to rebind the vao when we 
    // want to draw something that is given to the gpu.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    
    // Generate the vertices needed for a rectangle.
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
    };
    
    
    // Create a buffer and bind the vertices from the rectangle to the buffer as an array buffer.
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // This says how the incoming data is laid out, look at the vertex shader. Here we are 
    // saying that the vec3 on location = 0 will get 3 vec3's of float. The incoming data 
    // will not be normalized, and each individual size of the floats combined to be the 
    // vec3 will be 3 * the size of a float. the last parameter is the offset, we ignore 
    // it in this case because the first item will be also the the first vec3.x
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0); // This is saying, use the location = 0 now plz.
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // This is saying, use the location = 0 now plz.
    
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
    
    
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Render triangles as lines only.
    
    while (state->running) {
        processInput(state);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        useShader(shader);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
