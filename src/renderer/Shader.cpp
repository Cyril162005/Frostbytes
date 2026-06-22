#include "Shader.h"
#include "core/Logger.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace fb {

Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
    std::string vertSource = ReadFile(vertPath);
    std::string fragSource = ReadFile(fragPath);

    uint32_t vs = CompileStage(vertSource, GL_VERTEX_SHADER, vertPath);
    uint32_t fs = CompileStage(fragSource, GL_FRAGMENT_SHADER, fragPath);

    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vs);
    glAttachShader(m_ProgramID, fs);
    glLinkProgram(m_ProgramID);

    int success;
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        int maxLength = 0;
        glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> infoLog(maxLength);
        glGetProgramInfoLog(m_ProgramID, maxLength, &maxLength, &infoLog[0]);
        
        std::string log(infoLog.begin(), infoLog.end());
        FB_ERROR("Shader link failed: " + log);
        glDeleteProgram(m_ProgramID);
        glDeleteShader(vs);
        glDeleteShader(fs);
        throw std::runtime_error("Shader link failed: " + log);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    FB_INFO("Shader compiled and linked: " + vertPath + " + " + fragPath);
}

Shader::~Shader() {
    if (m_ProgramID != 0) {
        glDeleteProgram(m_ProgramID);
    }
}

void Shader::Bind() const {
    glUseProgram(m_ProgramID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetIntArray(const std::string& name, int* values, uint32_t count) {
    glUniform1iv(GetUniformLocation(name), count, values);
}

void Shader::SetFloat(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& v) {
    glUniform2f(GetUniformLocation(name), v.x, v.y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& v) {
    glUniform3f(GetUniformLocation(name), v.x, v.y, v.z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& v) {
    glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& m) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
}

std::string Shader::ReadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        FB_ERROR("Failed to open file: " + path);
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

uint32_t Shader::CompileStage(const std::string& source, uint32_t glShaderType, const std::string& debugName) {
    uint32_t shader = glCreateShader(glShaderType);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        int maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<char> infoLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

        std::string log(infoLog.begin(), infoLog.end());
        FB_ERROR("Shader compilation failed (" + debugName + "): " + log);
        glDeleteShader(shader);
        throw std::runtime_error("Shader compilation failed (" + debugName + "): " + log);
    }

    return shader;
}

int Shader::GetUniformLocation(const std::string& name) const {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    int location = glGetUniformLocation(m_ProgramID, name.c_str());
    if (location == -1) {
        FB_WARN("Uniform '" + name + "' not found!");
    }
    
    m_UniformLocationCache[name] = location;
    return location;
}

} // namespace fb
