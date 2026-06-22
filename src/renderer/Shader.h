#pragma once
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>
#include <cstdint>
#include "Lighting.h"

namespace fb {

class Shader {
public:
    Shader(const std::string& vertPath, const std::string& fragPath);
    ~Shader();

    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    void Bind() const;
    void Unbind() const;

    void SetInt(const std::string& name, int value);
    void SetIntArray(const std::string& name, int* values, uint32_t count);
    void SetFloat(const std::string& name, float value);
    void SetVec2(const std::string& name, const glm::vec2& v);
    void SetVec3(const std::string& name, const glm::vec3& v);
    void SetVec4(const std::string& name, const glm::vec4& v);
    void SetMat4(const std::string& name, const glm::mat4& m);

    void SetMaterial(const std::string& name, const Material& material);
    void SetDirLight(const std::string& name, const DirectionalLight& light);
    void SetPointLight(const std::string& name, const PointLight& light);

private:
    uint32_t m_ProgramID = 0;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;

    std::string ReadFile(const std::string& path);
    uint32_t    CompileStage(const std::string& source, uint32_t glShaderType,
                              const std::string& debugName);
    int         GetUniformLocation(const std::string& name) const;
};

} // namespace fb
