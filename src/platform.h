#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "opengl.h"
#include <string>

struct MemoryArena {
    long int size; 
    void* memory;
};

struct PlatformState {
    MemoryArena permStorage;
    MemoryArena tempStorage;
    bool running = false;
};

struct ApplicationState {
    GLFWwindow* window;
    bool initialized = false;
    MemoryArena gameMemory;
};

void on_application_startup(PlatformState* state);
void on_application_destroy(PlatformState* state);
void on_application_update(PlatformState* state);
void on_application_render(PlatformState* state);

void info_log(const std::string& message);
void error_log(const std::string& message);

#endif
