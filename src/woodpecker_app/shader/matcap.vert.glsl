#version 150 core

in vec4 vertexPosition;
uniform mat4 mvp;

void main() {
    // swizzle and normalize PGA point to LA position vector
    vec4 vpos = vertexPosition.yzwx;
    vpos *= inversesqrt(pow(vpos.w, 2));

    gl_Position = mvp * vpos;
}
