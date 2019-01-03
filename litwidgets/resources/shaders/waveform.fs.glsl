#version 150

uniform sampler2D uTexture;
uniform vec4 uBackgroundColor;
uniform vec4 uPrimaryColor;
uniform vec4 uSecondaryColor;

in vec2 UV;

out vec4 outColor;

void main() {
    float primaryThresh = texture(uTexture, vec2(UV.x, 0)).r;
    float secondaryThresh = texture(uTexture, vec2(UV.x, 0.5)).r;
    float intensity = abs(UV.y * 2 - 1);
    outColor = intensity < primaryThresh ? uPrimaryColor : vec4(uBackgroundColor.rgb, 0);
    outColor = intensity < secondaryThresh ? uSecondaryColor : outColor;
}
