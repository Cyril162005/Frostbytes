#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec2 v_TexCoord;

uniform vec3 u_LightDir = vec3(0.5, 0.5, 0.5);
uniform vec4 u_Color = vec4(1.0, 1.0, 1.0, 1.0);

void main() {
    float diff = max(dot(normalize(v_Normal), normalize(u_LightDir)), 0.2); // Simple diffuse + ambient
    color = u_Color * diff;
}
