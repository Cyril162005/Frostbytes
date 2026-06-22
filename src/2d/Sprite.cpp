#include "Sprite.h"
#include <glm/gtc/matrix_transform.hpp>

namespace fb {

std::shared_ptr<VertexArray> Sprite::s_QuadVAO = nullptr;

Sprite::Sprite(const std::shared_ptr<Texture>& texture)
    : m_Texture(texture) {
}

const glm::mat4& Sprite::GetTransform() const {
    if (m_Dirty) {
        m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f));
        m_Transform = glm::rotate(m_Transform, m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        m_Transform = glm::scale(m_Transform, glm::vec3(m_Scale, 1.0f));

        // Apply anchor point offset
        // A scale of 1.0 means the sprite is the size of the texture (or 1x1 in world units)
        // For now, let's assume the quad is 1x1 from -0.5 to 0.5
        // Wait, if quad is 0 to 1, anchor 0.5,0.5 means we translate by -0.5, -0.5
        glm::vec2 offset = -m_Anchor;
        m_Transform = glm::translate(m_Transform, glm::vec3(offset, 0.0f));

        m_Dirty = false;
    }
    return m_Transform;
}

void Sprite::Init() {
    if (s_QuadVAO) return;

    s_QuadVAO = std::make_shared<VertexArray>();

    // Quad vertices: Position (vec3), TexCoord (vec2)
    // Using a 1x1 quad from 0,0 to 1,1
    float vertices[] = {
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Bottom Left
        1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Top Right
        0.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };

    auto vbo = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
    vbo->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float2, "a_TexCoord" }
    });
    s_QuadVAO->AddVertexBuffer(vbo);

    uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };
    auto ibo = std::make_shared<IndexBuffer>(indices, 6);
    s_QuadVAO->SetIndexBuffer(ibo);
}

void Sprite::Shutdown() {
    s_QuadVAO.reset();
}

} // namespace fb
