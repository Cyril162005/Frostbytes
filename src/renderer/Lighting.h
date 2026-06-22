#pragma once
#include <glm/glm.hpp>

namespace fb {

struct Material {
    glm::vec3 Ambient   = glm::vec3(0.1f);
    glm::vec3 Diffuse   = glm::vec3(1.0f);
    glm::vec3 Specular  = glm::vec3(0.5f);
    float     Shininess = 32.0f;
};

struct DirectionalLight {
    glm::vec3 Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    glm::vec3 Color     = glm::vec3(1.0f);
    float     Intensity = 1.0f;
};

struct PointLight {
    glm::vec3 Position  = glm::vec3(0.0f);
    glm::vec3 Color     = glm::vec3(1.0f);
    float     Intensity = 1.0f;
    
    float Constant  = 1.0f;
    float Linear    = 0.09f;
    float Quadratic = 0.032f;
};

} // namespace fb
