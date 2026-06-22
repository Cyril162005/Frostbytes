#include "Mesh.h"
#include "renderer/Renderer.h"

namespace fb {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    m_VertexArray = std::make_shared<VertexArray>();

    m_VertexBuffer = std::make_shared<VertexBuffer>((float*)vertices.data(), (uint32_t)(vertices.size() * sizeof(Vertex)));
    m_VertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float3, "a_Normal"   },
        { ShaderDataType::Float2, "a_TexCoord" }
    });
    m_VertexArray->AddVertexBuffer(m_VertexBuffer);

    m_IndexBuffer = std::make_shared<IndexBuffer>((uint32_t*)indices.data(), (uint32_t)indices.size());
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
}

void Mesh::Draw() const {
    Renderer::DrawIndexed(m_VertexArray);
}

} // namespace fb
