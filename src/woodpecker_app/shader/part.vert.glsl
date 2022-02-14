#version 150 core

in vec4 vertexPosition;
uniform mat4 mvp;

void main() {
   gl_Position = mvp * vertexPosition;
}
