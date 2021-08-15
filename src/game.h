#ifndef _GAME_H_
#define _GAME_H_

#include "platform.h"

struct GameState {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int shader;
};

void on_viewport_resize(int width, int height);
void on_game_startup(ApplicationState* appState);
void on_game_destroy(ApplicationState* appState);
void on_game_update(ApplicationState* appState);
void on_game_render(ApplicationState* appState);

#endif
