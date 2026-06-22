#pragma once
#include <glm/glm.hpp>

namespace fb {

class Camera2D {
public:
    Camera2D(float left, float right, float bottom, float top);

    void SetProjection(float left, float right, float bottom, float top);

    const glm::vec2& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec2& position) { m_Position = position; RecalculateViewMatrix(); }

    float GetRotation() const { return m_Rotation; }
    void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

    float GetZoom() const { return m_Zoom; }
    void SetZoom(float zoom) { m_Zoom = zoom; RecalculateViewMatrix(); }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

private:
    void RecalculateViewMatrix();

private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec2 m_Position = { 0.0f, 0.0f };
    float     m_Rotation = 0.0f;
    float     m_Zoom     = 1.0f;
};

} // namespace fb
