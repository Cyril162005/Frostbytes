#pragma once
#include <string>
#include <cstdint>

struct GLFWwindow;   // forward declare — never include GLFW in headers

namespace fb {

class Window {
public:
    Window(const std::string& title, uint32_t width, uint32_t height);
    ~Window();

    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    bool        ShouldClose() const;
    void        PollEvents();
    void        SwapBuffers();
    void        SetTitle(const std::string& title);

    uint32_t     GetWidth()        const { return m_Width; }
    uint32_t     GetHeight()       const { return m_Height; }
    GLFWwindow*  GetNativeWindow() const { return m_Window; }

private:
    GLFWwindow* m_Window;
    uint32_t    m_Width, m_Height;
    std::string m_Title;
};

} // namespace fb
