#include "Application.h"
#include "Logger.h"
#include "platform/Window.h"
#include "renderer/Renderer.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cassert>

namespace fb {

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& title, uint32_t width, uint32_t height) {
    assert(!s_Instance);
    s_Instance = this;

    FB_INFO("Frostbytes Engine v0.1.0 starting...");
    m_Window = std::make_unique<Window>(title, width, height);

    Renderer::Init();

    m_TestShader = std::make_unique<Shader>("assets/shaders/test.vert", "assets/shaders/test.frag");
    
    // Create a 1x1 white texture
    uint32_t whiteData = 0xffffffff;
    m_WhiteTexture = std::make_shared<Texture>(1, 1, &whiteData);

    m_SpriteShader = std::make_unique<Shader>("assets/shaders/sprite.vert", "assets/shaders/sprite.frag");
    m_TestSprite = std::make_unique<Sprite>(m_WhiteTexture);
    m_TestSprite->SetPosition({ 400.0f, 300.0f });
    m_TestSprite->SetScale({ 100.0f, 100.0f });
    m_TestSprite->SetColor({ 1.0f, 0.5f, 0.2f, 1.0f }); // Orange-ish

    glGenVertexArrays(1, &m_EmptyVAO);
}

Application::~Application() {
    Renderer::Shutdown();
    glDeleteVertexArrays(1, &m_EmptyVAO);
    FB_INFO("Frostbytes Engine shutdown complete");
}

void Application::Run() {
    FB_INFO("Entering main loop");
    while (m_Running && !m_Window->ShouldClose()) {
        m_Window->PollEvents();

        // Update camera projection if window resized (simple check for now)
        m_Camera->SetProjection(0.0f, (float)m_Window->GetWidth(), 0.0f, (float)m_Window->GetHeight());
        
        Renderer::SetClearColor({ 0.08f, 0.08f, 0.10f, 1.0f });
        Renderer::Clear();
        
        float time = (float)glfwGetTime();
        m_TestShader->Bind();
        m_TestShader->SetFloat("u_Time", time);
        glBindVertexArray(m_EmptyVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 2. Render Sprite
        Renderer::BeginScene(*m_Camera);
        
        m_SpriteShader->Bind();
        m_SpriteShader->SetVec4("u_Color", m_TestSprite->GetColor());
        m_TestSprite->GetTexture()->Bind();
        
        Renderer::Submit(m_SpriteShader, Sprite::GetQuadVertexArray(), m_TestSprite->GetTransform());
        
        Renderer::EndScene();

        m_Window->SwapBuffers();
    }
}

} // namespace fb
