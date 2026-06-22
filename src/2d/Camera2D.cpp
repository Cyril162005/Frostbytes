#include "Camera2D.h"
#include <glm/gtc/matrix_transform.hpp>

namespace fb {

Camera2D::Camera2D(float left, float right, float bottom, float top)
    : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f) {
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera2D::SetProjection(float left, float right, float bottom, float top) {
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Camera2D::RecalculateViewMatrix() {
    // In 2D, the "view" is the inverse of the camera's transform
    // We translate by -position, rotate by -rotation, and scale by zoom
    
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position, 0.0f)) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(m_Zoom, m_Zoom, 1.0f));

    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

} // namespace fb
