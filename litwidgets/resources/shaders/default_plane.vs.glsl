#version 150

in vec3 inPos;
in vec2 inUV;

out vec2 UV;

void main() {
    UV = inUV;
    gl_Position = vec4(inPos, 1.0);
}
