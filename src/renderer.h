#ifndef _RENDERER_H_
#define _RENDERER_H_

int initialize_renderer();
void shutdown_renderer();

void clear_buffer(float r, float g, float b, float a); 
void submit_buffer();

void draw_rectangle(float x, float y, float width, float height);

#endif