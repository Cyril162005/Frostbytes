#pragma once
#include <string>
#include <cstdint>

namespace fb {

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }
    uint32_t GetRendererID() const { return m_RendererID; }

private:
    uint32_t m_RendererID;
    std::string m_Path;
    int m_Width, m_Height, m_BPP;
};

} // namespace fb
