#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec2 v_TexCoord;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 u_ViewPos;
uniform Material u_Material;
uniform DirLight u_DirLight;
uniform PointLight u_PointLights[8];
uniform int u_PointLightCount;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    
    vec3 ambient  = light.color * light.intensity * u_Material.ambient;
    vec3 diffuse  = light.color * light.intensity * diff * u_Material.diffuse;
    vec3 specular = light.color * light.intensity * spec * u_Material.specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    
    vec3 ambient  = light.color * light.intensity * u_Material.ambient;
    vec3 diffuse  = light.color * light.intensity * diff * u_Material.diffuse;
    vec3 specular = light.color * light.intensity * spec * u_Material.specular;
    
    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - v_WorldPos);
    
    // 1. Directional light
    vec3 result = CalcDirLight(u_DirLight, norm, viewDir);
    
    // 2. Point lights
    for(int i = 0; i < u_PointLightCount; i++) {
        result += CalcPointLight(u_PointLights[i], norm, v_WorldPos, viewDir);
    }
    
    color = vec4(result, 1.0);
}
