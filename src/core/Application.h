#pragma once
#include <memory>
#include <string>
#include <cstdint>

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

    static Application* s_Instance;
};

} // namespace fb
