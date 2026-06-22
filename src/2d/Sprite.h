#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include "renderer/Texture.h"
#include "renderer/VertexArray.h"

namespace fb {

class Sprite {
public:
    Sprite(const std::shared_ptr<Texture>& texture);
    ~Sprite() = default;

    void SetPosition(const glm::vec2& position) { m_Position = position; m_Dirty = true; }
    void SetRotation(float rotation) { m_Rotation = rotation; m_Dirty = true; }
    void SetScale(const glm::vec2& scale) { m_Scale = scale; m_Dirty = true; }
    void SetColor(const glm::vec4& color) { m_Color = color; }
    void SetAnchor(const glm::vec2& anchor) { m_Anchor = anchor; m_Dirty = true; }

    const glm::vec2& GetPosition() const { return m_Position; }
    float GetRotation() const { return m_Rotation; }
    const glm::vec2& GetScale() const { return m_Scale; }
    const glm::vec4& GetColor() const { return m_Color; }
    const glm::vec2& GetAnchor() const { return m_Anchor; }

    const glm::mat4& GetTransform() const;
    const std::shared_ptr<Texture>& GetTexture() const { return m_Texture; }

    static void Init();
    static void Shutdown();
    static const std::shared_ptr<VertexArray>& GetQuadVertexArray() { return s_QuadVAO; }

private:
    std::shared_ptr<Texture> m_Texture;
    
    glm::vec2 m_Position{ 0.0f, 0.0f };
    float     m_Rotation{ 0.0f };
    glm::vec2 m_Scale{ 1.0f, 1.0f };
    glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec2 m_Anchor{ 0.5f, 0.5f }; // Default to center

    mutable glm::mat4 m_Transform{ 1.0f };
    mutable bool      m_Dirty{ true };

    static std::shared_ptr<VertexArray> s_QuadVAO;
};

} // namespace fb
