PROJECT: Frostbytes Game Engine — Step 1 of 17: Project Foundation

You are an expert C++ systems developer. Build the complete project skeleton for
"Frostbytes" — a C++17 game engine targeting 2D/3D (2005-2015 era fidelity) on
Intel HD 4000 GPU, Intel Core i3, 8GB RAM.

Namespace for all code: fb
No rendering code yet — just the window, context, logger, and app loop.

━━━ TECH STACK ━━━
  Language:   C++17
  Graphics:   OpenGL 3.3 Core Profile
  Windowing:  GLFW 3.3.8 (CMake FetchContent)
  GL Loader:  GLAD v2.0.4 (CMake FetchContent, SOURCE_SUBDIR cmake)
  Math:       GLM 0.9.9.8 (CMake FetchContent)
  Build:      CMake 3.20+

━━━ CREATE EXACTLY THIS STRUCTURE ━━━

  Frostbytes/
  ├── CMakeLists.txt
  ├── src/
  │   ├── main.cpp
  │   ├── core/
  │   │   ├── Application.h
  │   │   ├── Application.cpp
  │   │   ├── Logger.h
  │   │   └── Logger.cpp
  │   └── platform/
  │       ├── Window.h
  │       └── Window.cpp
  ├── vendor/
  │   └── .gitkeep
  └── assets/
      └── .gitkeep

━━━ FILE SPECIFICATIONS ━━━

── CMakeLists.txt ──────────────────────────────────────────────────────────────

cmake_minimum_required(VERSION 3.20)
project(Frostbytes VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD 2
FetchContent_Declare(
    glad2
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        v2.0.4
    SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glad2)
glad_add_library(glad_gl STATIC LANGUAGE C API gl:core=3.3)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

file(GLOB_RECURSE FB_SOURCES "src/*.cpp")
add_executable(Frostbytes ${FB_SOURCES})

target_link_libraries(Frostbytes PRIVATE glfw glad_gl glm::glm)
target_include_directories(Frostbytes PRIVATE src vendor)

if(MSVC)
    target_compile_options(Frostbytes PRIVATE /W4)
else()
    target_compile_options(Frostbytes PRIVATE -Wall -Wextra -Wpedantic)
endif()

── src/core/Logger.h ───────────────────────────────────────────────────────────

#pragma once
#include <string>

namespace fb {

enum class LogLevel { Info, Warn, Error };

class Logger {
public:
    static void Log(LogLevel level, const std::string& msg,
                    const char* file, int line);
};

} // namespace fb

#define FB_INFO(msg)  fb::Logger::Log(fb::LogLevel::Info,  (msg), __FILE__, __LINE__)
#define FB_WARN(msg)  fb::Logger::Log(fb::LogLevel::Warn,  (msg), __FILE__, __LINE__)
#define FB_ERROR(msg) fb::Logger::Log(fb::LogLevel::Error, (msg), __FILE__, __LINE__)

── src/core/Logger.cpp ─────────────────────────────────────────────────────────

Implement Logger::Log with these rules:
- Get current time via std::chrono + std::localtime, format as [HH:MM:SS]
- Output format:  [HH:MM:SS] [LEVEL] (filename:line) message
  - filename: basename only — strip path separators (both / and \)
- ANSI color codes: INFO=\033[32m  WARN=\033[33m  ERROR=\033[31m  reset=\033[0m
- INFO + WARN write to std::cout, ERROR writes to std::cerr
- On Windows (_MSC_VER), skip ANSI codes unless you enable virtual terminal

── src/core/Application.h ──────────────────────────────────────────────────────

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

── src/core/Application.cpp ────────────────────────────────────────────────────

Include: Application.h, Logger.h, platform/Window.h, <glad/gl.h>

- Initialize:  Application::s_Instance = nullptr  (define it in .cpp)
- Constructor: assert(!s_Instance), set s_Instance=this,
               FB_INFO("Frostbytes Engine v0.1.0 starting..."),
               m_Window = std::make_unique<Window>(title, width, height)
- Destructor:  FB_INFO("Frostbytes Engine shutdown complete")
- Run():       FB_INFO("Entering main loop"),
               while(m_Running && !m_Window->ShouldClose()) {
                   m_Window->PollEvents();
                   glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
                   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                   m_Window->SwapBuffers();
               }

── src/platform/Window.h ───────────────────────────────────────────────────────

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
    GLFWwindow*  m_Window = nullptr;
    uint32_t     m_Width, m_Height;
    std::string  m_Title;
};

} // namespace fb

── src/platform/Window.cpp ─────────────────────────────────────────────────────

IMPORTANT include order (MUST be exactly this):
  #include <glad/gl.h>      // always before GLFW
  #include <GLFW/glfw3.h>
  #include "Window.h"
  #include "core/Logger.h"
  #include <string>
  #include <stdexcept>

Constructor steps (in order):
  1. glfwSetErrorCallback([](int err, const char* desc){
         FB_ERROR("GLFW error " + std::to_string(err) + ": " + desc);
     });
  2. if(!glfwInit()) throw std::runtime_error("Failed to init GLFW");
  3. glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
     #ifdef __APPLE__
         glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
     #endif
  4. m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
     if(!m_Window) throw std::runtime_error("Failed to create GLFW window");
  5. glfwMakeContextCurrent(m_Window);
  6. if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
         throw std::runtime_error("Failed to initialize GLAD");
  7. glViewport(0, 0, width, height);
     glfwSwapInterval(1);  // vsync on
  8. glfwSetFramebufferSizeCallback(m_Window,
         [](GLFWwindow*, int w, int h){ glViewport(0, 0, w, h); });
  9. glfwSetKeyCallback(m_Window,
         [](GLFWwindow* win, int key, int, int action, int){
             if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                 glfwSetWindowShouldClose(win, GLFW_TRUE);
         });
  10. FB_INFO("Window:  " + title + "  [" + std::to_string(width)
              + "x" + std::to_string(height) + "]");
      FB_INFO("OpenGL:  " + std::string((const char*)glGetString(GL_VERSION)));
      FB_INFO("GPU:     " + std::string((const char*)glGetString(GL_RENDERER)));

Destructor: glfwDestroyWindow(m_Window);  glfwTerminate();
ShouldClose:  return glfwWindowShouldClose(m_Window);
PollEvents:   glfwPollEvents();
SwapBuffers:  glfwSwapBuffers(m_Window);
SetTitle:     glfwSetWindowTitle(m_Window, title.c_str()); m_Title = title;

── src/main.cpp ────────────────────────────────────────────────────────────────

#include "core/Application.h"
#include "core/Logger.h"
#include <exception>
#include <iostream>

int main() {
    try {
        fb::Application app("Frostbytes Engine", 1280, 720);
        app.Run();
    } catch (const std::exception& e) {
        std::cerr << "[FATAL] " << e.what() << "\n";
        return 1;
    }
    return 0;
}

━━━ BUILD COMMANDS ━━━

  mkdir build && cd build
  cmake .. -DCMAKE_BUILD_TYPE=Debug
  cmake --build . --config Debug

━━━ EXPECTED OUTPUT ━━━

Console:
  [HH:MM:SS] [INFO]  Frostbytes Engine v0.1.0 starting...
  [HH:MM:SS] [INFO]  Window:  Frostbytes Engine  [1280x720]
  [HH:MM:SS] [INFO]  OpenGL:  3.3.0 - Build <number>
  [HH:MM:SS] [INFO]  GPU:     Intel(R) HD Graphics 4000
  [HH:MM:SS] [INFO]  Entering main loop...
  [HH:MM:SS] [INFO]  Frostbytes Engine shutdown complete

Window: 1280x720, very dark background (#141417), ESC or X closes it.

Zero compiler warnings. Zero memory leaks.