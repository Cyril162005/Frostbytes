#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "renderer/VertexArray.h"

namespace fb {

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    ~Mesh() = default;

    void Draw() const;

    const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }

private:
    std::shared_ptr<VertexArray> m_VertexArray;
    std::shared_ptr<VertexBuffer> m_VertexBuffer;
    std::shared_ptr<IndexBuffer> m_IndexBuffer;
};

} // namespace fb
