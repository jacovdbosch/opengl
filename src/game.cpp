#include "opengl.h"
#include "game.h"
#include "renderer.h"

void on_viewport_resize(int width, int height) {
    glViewport(0, 0, width, height);
}

void on_game_startup(ApplicationState* appState) {
    int result = initialize_renderer();

    if (result != 0) {
        glfwSetWindowShouldClose(appState->window, GLFW_TRUE);

        return;
    }

    // draw_rectangle(100, 100, 300, 300);

    // Setup the buffers.
    // float data[] = {
    //      0.0f,  0.5f, 1.0f, 0.0f, 0.0f, 
    //      0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 
    //     -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    // };

    // int elements[] = {
    //     0, 1, 2,
    // };
}

void on_game_destroy(ApplicationState* appState) {
    shutdown_renderer();
}

void on_game_update(ApplicationState* appState) {
    if (glfwGetKey(appState->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(appState->window, GLFW_TRUE);

        return;
    }
}

void on_game_render(ApplicationState* appState) {
    clear_buffer(0.2f, 0.3f, 0.3f, 1.0f);

    // draw_rectangle(100, 100, 300, 300);

    submit_buffer();
}
