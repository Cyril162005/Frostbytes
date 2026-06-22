#include <glad/gl.h>      // always before GLFW
#include <GLFW/glfw3.h>
#include "Window.h"
#include "core/Logger.h"
#include <string>
#include <stdexcept>

namespace fb {

Window::Window(const std::string& title, uint32_t width, uint32_t height)
    : m_Width(width), m_Height(height), m_Title(title) {
    
    glfwSetErrorCallback([](int err, const char* desc) {
        FB_ERROR("GLFW error " + std::to_string(err) + ": " + desc);
    });

    if (!glfwInit()) {
        throw std::runtime_error("Failed to init GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width, height);
    glfwSwapInterval(1);  // vsync on

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* win, int key, int, int action, int) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(win, GLFW_TRUE);
        }
    });

    FB_INFO("Window:  " + title + "  [" + std::to_string(width) + "x" + std::to_string(height) + "]");
    FB_INFO("OpenGL:  " + std::string((const char*)glGetString(GL_VERSION)));
    FB_INFO("GPU:     " + std::string((const char*)glGetString(GL_RENDERER)));
}

Window::~Window() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

void Window::PollEvents() {
    glfwPollEvents();
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}

void Window::SetTitle(const std::string& title) {
    glfwSetWindowTitle(m_Window, title.c_str());
    m_Title = title;
}

} // namespace fb
