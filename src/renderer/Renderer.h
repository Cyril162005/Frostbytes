#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "VertexArray.h"
#include "Shader.h"
#include "2d/Camera2D.h"

namespace fb {

class Renderer {
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void SetClearColor(const glm::vec4& color);
    static void Clear();

    static void BeginScene(const Camera2D& camera);
    static void EndScene();

    static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f), const std::string& transformUniform = "u_Transform");

    // Internal draw call
    static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
};

} // namespace fb
