#include "Renderer2D.h"
#include <vector>
#include <array>
#include "renderer/Renderer.h"
#include <glad/gl.h>

namespace fb {

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float     TexIndex;
};

struct Renderer2DData {
    static const uint32_t MaxQuads = 10000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32;

    std::shared_ptr<VertexArray>  QuadVAO;
    std::shared_ptr<VertexBuffer> QuadVBO;
    std::shared_ptr<Shader>       SpriteShader;
    std::shared_ptr<Texture>      WhiteTexture;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    std::array<std::shared_ptr<Texture>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // 0 = white texture

    glm::vec4 QuadVertexPositions[4];
};

static Renderer2DData s_Data;

void Renderer2D::Init() {
    s_Data.QuadVAO = std::make_shared<VertexArray>();

    s_Data.QuadVBO = std::make_shared<VertexBuffer>(s_Data.MaxVertices * sizeof(QuadVertex));
    s_Data.QuadVBO->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" },
        { ShaderDataType::Float2, "a_TexCoord" },
        { ShaderDataType::Float,  "a_TexIndex" }
    });
    s_Data.QuadVAO->AddVertexBuffer(s_Data.QuadVBO);

    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

    uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    auto ibo = std::make_shared<IndexBuffer>(quadIndices, s_Data.MaxIndices);
    s_Data.QuadVAO->SetIndexBuffer(ibo);
    delete[] quadIndices;

    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture = std::make_shared<Texture>(1, 1, &whiteTextureData);

    int32_t samplers[s_Data.MaxTextureSlots];
    for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        samplers[i] = i;

    s_Data.SpriteShader = std::make_shared<Shader>("assets/shaders/sprite.vert", "assets/shaders/sprite.frag");
    s_Data.SpriteShader->Bind();
    s_Data.SpriteShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

    s_Data.TextureSlots[0] = s_Data.WhiteTexture;

    s_Data.QuadVertexPositions[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[1] = { 1.0f, 0.0f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[2] = { 1.0f, 1.0f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[3] = { 0.0f, 1.0f, 0.0f, 1.0f };
}

void Renderer2D::Shutdown() {
    delete[] s_Data.QuadVertexBufferBase;
}

void Renderer2D::BeginScene(const Camera2D& camera) {
    s_Data.SpriteShader->Bind();
    s_Data.SpriteShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    StartBatch();
}

void Renderer2D::EndScene() {
    Flush();
}

void Renderer2D::StartBatch() {
    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
    s_Data.TextureSlotIndex = 1;
}

void Renderer2D::Flush() {
    if (s_Data.QuadIndexCount == 0)
        return;

    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
    s_Data.QuadVBO->SetData(s_Data.QuadVertexBufferBase, dataSize);

    for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        s_Data.TextureSlots[i]->Bind(i);

    s_Data.QuadVAO->Bind();
    glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
}

void Renderer2D::NextBatch() {
    Flush();
    StartBatch();
}

void Renderer2D::DrawSprite(const Sprite& sprite) {
    DrawSprite(sprite.GetTransform(), sprite.GetTexture(), sprite.GetColor());
}

void Renderer2D::DrawSprite(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture>& texture, const glm::vec4& color) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                          glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
    DrawSprite(transform, texture, color);
}

void Renderer2D::DrawSprite(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& color) {
    if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        NextBatch();

    float textureIndex = 0.0f;
    for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
        if (*s_Data.TextureSlots[i] == *texture) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
            NextBatch();

        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    for (size_t i = 0; i < quadVertexCount; i++) {
        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;
}

} // namespace fb
