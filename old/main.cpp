#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

struct Application {
    GLFWwindow* window;
    bool running = true;
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

Application* createApplication() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    int xpos, ypos, width, height;
    glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &width, &height);


    std::cout << "Width: " << width << ", height: " << height << std::endl;


    GLFWwindow* window = glfwCreateWindow(width, height, "Learn Opengl", NULL, NULL);
    
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
    
    framebufferSizeCallback(window, width, height);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    return new Application { window };
}


int main() {
    Application* application = createApplication();


    while(!glfwWindowShouldClose(application->window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(application->window);
        glfwPollEvents();
    }

    glfwTerminate();
    delete application;

    return 0;
}
