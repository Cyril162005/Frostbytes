#include "Application.h"
#include "Logger.h"
#include "platform/Window.h"
#include "renderer/Shader.h"
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

    m_TestShader = std::make_unique<Shader>("assets/shaders/test.vert", "assets/shaders/test.frag");
    
    glGenVertexArrays(1, &m_EmptyVAO);
}

Application::~Application() {
    glDeleteVertexArrays(1, &m_EmptyVAO);
    FB_INFO("Frostbytes Engine shutdown complete");
}

void Application::Run() {
    FB_INFO("Entering main loop");
    while (m_Running && !m_Window->ShouldClose()) {
        m_Window->PollEvents();
        
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float time = (float)glfwGetTime();
        m_TestShader->Bind();
        m_TestShader->SetFloat("u_Time", time);
        glBindVertexArray(m_EmptyVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        m_Window->SwapBuffers();
    }
}

} // namespace fb
