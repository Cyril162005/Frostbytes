#include "Renderer.h"
#include <glad/gl.h>
#include "2d/Sprite.h"

namespace fb {

struct RendererData {
    glm::mat4 ViewProjectionMatrix;
};

static RendererData s_Data;

void Renderer::Init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    Sprite::Init();
}

void Renderer::Shutdown() {
    Sprite::Shutdown();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
    glViewport(0, 0, width, height);
}

void Renderer::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::BeginScene(const Camera2D& camera) {
    s_Data.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {
}

void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform, const std::string& transformUniform) {
    shader->Bind();
    shader->SetMat4("u_ViewProjection", s_Data.ViewProjectionMatrix);
    if (!transformUniform.empty()) {
        shader->SetMat4(transformUniform, transform);
    }
    DrawIndexed(vertexArray);
}

void Renderer::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) {
    vertexArray->Bind();
    uint32_t count = vertexArray->GetIndexBuffer() ? vertexArray->GetIndexBuffer()->GetCount() : 0;
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

} // namespace fb
