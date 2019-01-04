#version 150

const float cMarkerBorderWidth = 0.002;
const int cMarkerCount = 10;

uniform sampler2D uTexture;
uniform vec4 uBackgroundColor;
uniform vec4 uPrimaryColor;
uniform vec4 uSecondaryColor;
uniform vec4 uFocusBorderColor;
uniform vec2 uFocusWindow;
uniform vec4 uMarkerBorderColor;
uniform float uMarkers[cMarkerCount];
uniform vec4 uCursorBorderColor;
uniform float uCursor;

in vec2 UV;

out vec4 outColor;

void main() {
    float x = UV.x;
    float primaryThresh = texture(uTexture, vec2(x, 0)).r;
    float secondaryThresh = texture(uTexture, vec2(x, 0.5)).r;
    float intensity = abs(UV.y * 2 - 1);
    float opacity = x > uFocusWindow.x && x < uFocusWindow.y ? 1 : 0.5;
    outColor = vec4(uBackgroundColor.rgb, 0);
    outColor = intensity < primaryThresh ? uPrimaryColor * opacity : outColor;
    outColor = intensity < secondaryThresh ? uSecondaryColor * opacity : outColor;
    outColor = abs(x - uFocusWindow.x) < cMarkerBorderWidth || abs(x - uFocusWindow.y) < cMarkerBorderWidth ? uFocusBorderColor : outColor;

    for(int i = 0; i < cMarkerCount; ++i) {
        outColor = abs(x - uMarkers[i]) < cMarkerBorderWidth ? uMarkerBorderColor : outColor;
    }
    outColor = abs(x - uCursor) < cMarkerBorderWidth ? uCursorBorderColor : outColor;
}
