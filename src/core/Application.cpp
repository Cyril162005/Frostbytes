#include "Application.h"
#include "Logger.h"
#include "platform/Window.h"
#include "renderer/Renderer.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
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
        
        Renderer::SetClearColor({ 0.08f, 0.08f, 0.10f, 1.0f });
        Renderer::Clear();
        
        // 1. Render attributeless triangle
        float time = (float)glfwGetTime();
        m_TestShader->Bind();
        m_TestShader->SetFloat("u_Time", time);
        glBindVertexArray(m_EmptyVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 2. Render Sprite
        Renderer::BeginScene();
        
        // Simple orthographic projection: 0 to width, 0 to height
        glm::mat4 projection = glm::ortho(0.0f, (float)m_Window->GetWidth(), 0.0f, (float)m_Window->GetHeight(), -1.0f, 1.0f);
        
        m_SpriteShader->Bind();
        m_SpriteShader->SetMat4("u_ViewProjection", projection);
        m_SpriteShader->SetVec4("u_Color", m_TestSprite->GetColor());
        m_TestSprite->GetTexture()->Bind();
        
        Renderer::Submit(m_SpriteShader, Sprite::GetQuadVertexArray(), m_TestSprite->GetTransform());
        
        Renderer::EndScene();

        m_Window->SwapBuffers();
    }
}

} // namespace fb
