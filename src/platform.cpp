#include "platform.h"
#include "game.h"
#include <stdio.h>

int main() {
    PlatformState* state = (PlatformState*)malloc(sizeof(PlatformState));

    state->permStorage.size = megabytes(40);
    state->permStorage.memory = calloc(1, state->permStorage.size);

    if (state->permStorage.memory == NULL) {
        error_log("Failed to initialize the permanent storage");
        std::cout << "Failed to initialize the perm storage" << std::endl;

        return -1;
    }

    state->tempStorage.size = megabytes(40);
    state->tempStorage.memory = calloc(1, state->tempStorage.size);

    if (state->tempStorage.memory == NULL) {
        error_log("Failed to initialize the temporary storage");

        return -1;
    }

    on_application_startup(state);

    state->running = true;

    while (state->running) {
        on_application_update(state);
        on_application_render(state);
    }

    on_application_destroy(state);

    free(state->permStorage.memory);
    free(state->tempStorage.memory);
    free(state);

    return 0;
}

char* load_file(const char* filePath) {
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

void info_log(const std::string& message) {
    std::cout << "INFO: " << message << std::endl;
}

void error_log(const std::string& message) {
    std::cout << "ERROR: " << message << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    on_viewport_resize(width, height);
}

void on_application_startup(PlatformState* state) {
    ApplicationState* appState = (ApplicationState*)state->permStorage.memory;

    appState->gameMemory.size = megabytes(40);
    appState->gameMemory.memory = calloc(1, appState->gameMemory.size);

    if (appState->gameMemory.memory == NULL) {
        std::cout << "Failed to initialize game storage" << std::endl;

        appState->initialized = false;
        return;
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int width = 800;
    const int height = 600;

    GLFWwindow* window = glfwCreateWindow(width, height, "Learn Opengl", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create the GLFW window" << std::endl;

        appState->initialized = false;
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;

        appState->initialized = false;
        return;
    }

    appState->window = window;

    appState->initialized = true;

    on_game_startup(appState);

    framebuffer_size_callback(window, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void on_application_destroy(PlatformState* state) {
    ApplicationState* appState = (ApplicationState*)state->permStorage.memory;

    on_game_destroy(appState);
    glfwTerminate();

    free(appState->gameMemory.memory);
}

void on_application_update(PlatformState* state) {
    ApplicationState* appState = (ApplicationState*)state->permStorage.memory;

    if (!appState->initialized) {
        state->running = false;

        return;
    }

    if (glfwWindowShouldClose(appState->window)) {
        state->running = false;

        return;
    }

    on_game_update(appState);

    glfwPollEvents();
}

void on_application_render(PlatformState* state) {
    ApplicationState* appState = (ApplicationState*)state->permStorage.memory;

    if (!appState->initialized) {
        state->running = false;

        return;
    }

    on_game_render(appState);
    glfwSwapBuffers(appState->window);
}
