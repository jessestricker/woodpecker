#version 150 core

out vec3 fragColor;
uniform vec3 faceColor;

void main() {
    fragColor = faceColor;
    // TODO: line color
}
