#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "2d/Camera2D.h"
#include "2d/Sprite.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"

namespace fb {

class Renderer2D {
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera2D& camera);
    static void EndScene();
    static void Flush();

    static void DrawSprite(const Sprite& sprite);
    static void DrawSprite(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, const glm::vec4& color = glm::vec4(1.0f));
    static void DrawSprite(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& color = glm::vec4(1.0f));

private:
    static void StartBatch();
    static void NextBatch();
};

} // namespace fb
