#version 330 core

in vec3 vColor;
out vec4 FragColor;

uniform float u_Time;

void main() {
    // Pulse brightness with time to prove the uniform pipeline works end to end
    float pulse = 0.7 + 0.3 * sin(u_Time);
    FragColor = vec4(vColor * pulse, 1.0);
}
