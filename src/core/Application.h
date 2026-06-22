#pragma once
#include <memory>
#include <string>
#include <cstdint>
#include "renderer/Shader.h"
#include "2d/Sprite.h"

namespace fb {

class Window;

class Application {
public:
    Application(const std::string& title, uint32_t width, uint32_t height);
    virtual ~Application();

    Application(const Application&)            = delete;
    Application& operator=(const Application&) = delete;

    void Run();
    void Close() { m_Running = false; }

    Window&           GetWindow() { return *m_Window; }
    static Application& Get()    { return *s_Instance; }

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;

    // Test assets
    std::unique_ptr<Shader> m_TestShader;
    std::unique_ptr<Shader> m_SpriteShader;
    std::unique_ptr<Sprite> m_TestSprite;
    std::shared_ptr<Texture> m_WhiteTexture;

    uint32_t m_EmptyVAO = 0;

    static Application* s_Instance;
};

} // namespace fb
