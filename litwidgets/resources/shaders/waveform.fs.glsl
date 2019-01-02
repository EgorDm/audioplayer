#version 150

in vec2 UV;

out vec4 outColor;

void main() {
    outColor = vec4(UV.x, 0, UV.y, 0);
}
