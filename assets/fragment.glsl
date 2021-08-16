#version 330 core

in vec3 vertexColor;

out vec4 gl_Color;

void main() {
    gl_Color = vec4(vertexColor, 1.0);
}
