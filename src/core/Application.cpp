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

    m_TestSprite = std::make_unique<Sprite>(m_WhiteTexture);
    m_TestSprite->SetPosition({ 400.0f, 300.0f });
    m_TestSprite->SetScale({ 100.0f, 100.0f });
    m_TestSprite->SetColor({ 1.0f, 0.5f, 0.2f, 1.0f }); // Orange-ish

    m_Camera = std::make_unique<Camera2D>(0.0f, (float)width, 0.0f, (float)height);

    // Create a cube mesh
    std::vector<Vertex> vertices = {
        // Front
        { {-0.5f, -0.5f,  0.5f}, {0, 0, 1}, {0, 0} },
        { { 0.5f, -0.5f,  0.5f}, {0, 0, 1}, {1, 0} },
        { { 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1, 1} },
        { {-0.5f,  0.5f,  0.5f}, {0, 0, 1}, {0, 1} },
        // Back
        { {-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1, 0} },
        { { 0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0} },
        { { 0.5f,  0.5f, -0.5f}, {0, 0, -1}, {0, 1} },
        { {-0.5f,  0.5f, -0.5f}, {0, 0, -1}, {1, 1} }
    };
    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0, // Front
        1, 5, 6, 6, 2, 1, // Right
        5, 4, 7, 7, 6, 5, // Back
        4, 0, 3, 3, 7, 4, // Left
        3, 2, 6, 6, 7, 3, // Top
        4, 5, 1, 1, 0, 4  // Bottom
    };
    m_TestMesh = std::make_unique<Mesh>(vertices, indices);
    m_MeshShader = std::make_unique<Shader>("assets/shaders/mesh.vert", "assets/shaders/mesh.frag");

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
        
        // 1. Render attributeless triangle
        float time = (float)glfwGetTime();
        m_TestShader->Bind();
        m_TestShader->SetFloat("u_Time", time);
        glBindVertexArray(m_EmptyVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 2. Render Sprite via Renderer2D (Batching)
        Renderer2D::BeginScene(*m_Camera);
        
        // Draw the main test sprite
        Renderer2D::DrawSprite(*m_TestSprite);

        // Draw a bunch of random sprites to test batching
        for (float y = -2.0f; y < 2.0f; y += 0.5f) {
            for (float x = -2.0f; x < 2.0f; x += 0.5f) {
                glm::vec4 color = { (x + 2.0f) / 4.0f, 0.4f, (y + 2.0f) / 4.0f, 0.7f };
                Renderer2D::DrawSprite({ 400.0f + x * 100.0f, 300.0f + y * 100.0f }, { 40.0f, 40.0f }, m_WhiteTexture, color);
            }
        }
        
        Renderer2D::EndScene();

        m_Window->SwapBuffers();
    }
}

} // namespace fb
