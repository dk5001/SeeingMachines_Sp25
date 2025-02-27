#version 150

uniform vec2 resolution;
uniform vec4 particleColor; // Color uniform

out vec4 outputColor;

void main() {
    vec2 st = gl_FragCoord.xy / resolution.xy;

    // Create a more intense glow effect by adding a blurred outline
    vec4 glow = vec4(0.0);
    for (float x = -10.0; x <= 10.0; x += 1.0) {
        for (float y = -10.0; y <= 10.0; y += 1.0) {
            vec2 offset = vec2(x, y) / resolution;
            glow += particleColor / (1.0 + length(offset) * 5.0);
        }
    }
    glow /= 441.0; // Normalize the glow effect (21x21 = 441)

    // Apply the particle color and more intense glow
    outputColor = particleColor + glow * 1.0;
}
