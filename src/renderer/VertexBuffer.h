#pragma once
#include <cstdint>
#include "BufferLayout.h"

namespace fb {

class VertexBuffer {
public:
    VertexBuffer(float* vertices, uint32_t size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    const BufferLayout& GetLayout() const { return m_Layout; }
    void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

private:
    uint32_t m_RendererID;
    BufferLayout m_Layout;
};

} // namespace fb
