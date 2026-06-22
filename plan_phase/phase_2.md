PROJECT: Frostbytes Game Engine — Step 2 of 17: Shader System + GLSL Pipeline

Continuing the Frostbytes engine. Step 1 (window + context + logger) is complete
and verified working. Now build the shader abstraction layer.

Namespace: fb
Do NOT touch any existing files except CMakeLists.txt (only to add the new
source files via the existing GLOB_RECURSE — no manual changes needed there
unless shaders need to be copied to the build output, see below).

━━━ GOAL ━━━

A Shader class that loads, compiles, links, and binds GLSL programs, with a
typed uniform-setting API using GLM. Test it by rendering a single hardcoded
triangle using "attributeless rendering" — no VBO/VAO data upload yet (that's
Step 3). The triangle's vertex positions are generated entirely inside the
vertex shader using gl_VertexID, so OpenGL still requires a bound (but empty)
VAO to issue the draw call — that's expected and correct for 3.3 Core Profile.

━━━ NEW FILES ━━━

  src/renderer/Shader.h
  src/renderer/Shader.cpp
  assets/shaders/test.vert
  assets/shaders/test.frag

━━━ FILE SPECIFICATIONS ━━━

── src/renderer/Shader.h ───────────────────────────────────────────────────────

#pragma once
#include <string>
#include <glm/glm.hpp>

namespace fb {

class Shader {
public:
    Shader(const std::string& vertPath, const std::string& fragPath);
    ~Shader();

    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    void Bind() const;
    void Unbind() const;

    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVec2(const std::string& name, const glm::vec2& v);
    void SetVec3(const std::string& name, const glm::vec3& v);
    void SetVec4(const std::string& name, const glm::vec4& v);
    void SetMat4(const std::string& name, const glm::mat4& m);

private:
    uint32_t m_ProgramID = 0;

    std::string ReadFile(const std::string& path);
    uint32_t    CompileStage(const std::string& source, uint32_t glShaderType,
                              const std::string& debugName);
    int         GetUniformLocation(const std::string& name);
};

} // namespace fb

── src/renderer/Shader.cpp ─────────────────────────────────────────────────────

Include: Shader.h, core/Logger.h, <glad/gl.h>, <fstream>, <sstream>,
         <unordered_map>, <stdexcept>

Constructor:
  1. Read both files via ReadFile (throw std::runtime_error with the path if
     ifstream fails to open — do not silently continue)
  2. CompileStage both as GL_VERTEX_SHADER and GL_FRAGMENT_SHADER
  3. m_ProgramID = glCreateProgram(); attach both; glLinkProgram
  4. Check GL_LINK_STATUS — on failure, pull glGetProgramInfoLog, FB_ERROR it,
     throw std::runtime_error("Shader link failed: " + log)
  5. Delete both shader stage objects after linking (glDeleteShader) — they're
     no longer needed once linked into the program
  6. FB_INFO("Shader compiled and linked: " + vertPath + " + " + fragPath)

CompileStage:
  - glCreateShader, glShaderSource, glCompileShader
  - Check GL_COMPILE_STATUS — on failure, pull glGetShaderInfoLog, FB_ERROR it
    with debugName included, throw std::runtime_error
  - Return the shader ID

Destructor: glDeleteProgram(m_ProgramID) if non-zero

Bind:   glUseProgram(m_ProgramID)
Unbind: glUseProgram(0)

GetUniformLocation:
  - Cache results in a private std::unordered_map<std::string, int> member
    (add this member to the header) to avoid repeated glGetUniformLocation calls
  - If location == -1, FB_WARN once that the uniform name wasn't found (don't
    spam — only warn the first time per name, track via the same cache)

All SetX functions: Bind() first, then call the matching glUniformX /
glUniformMatrix4fv (use GL_FALSE for transpose, glm::value_ptr for the matrix
— remember #include <glm/gtc/type_ptr.hpp>)

── assets/shaders/test.vert ────────────────────────────────────────────────────

#version 330 core

// Attributeless triangle — positions baked in via gl_VertexID, no VBO needed
const vec2 positions[3] = vec2[](
    vec2( 0.0,  0.6),
    vec2(-0.6, -0.6),
    vec2( 0.6, -0.6)
);

const vec3 colors[3] = vec3[](
    vec3(1.0, 0.3, 0.3),
    vec3(0.3, 1.0, 0.3),
    vec3(0.3, 0.3, 1.0)
);

out vec3 vColor;

void main() {
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    vColor = colors[gl_VertexID];
}

── assets/shaders/test.frag ────────────────────────────────────────────────────

#version 330 core

in vec3 vColor;
out vec4 FragColor;

uniform float u_Time;

void main() {
    // Pulse brightness with time to prove the uniform pipeline works end to end
    float pulse = 0.7 + 0.3 * sin(u_Time);
    FragColor = vec4(vColor * pulse, 1.0);
}

━━━ MODIFY: src/core/Application.cpp ──────────────────────────────────────────

Add to the includes: "renderer/Shader.h", <glad/gl.h> (if not already there),
<GLFW/glfw3.h> only if needed for glfwGetTime — prefer std::chrono instead to
keep GLFW out of Application.

In the Application class (add to header too):
  - Add member: std::unique_ptr<Shader> m_TestShader;
  - Add member: std::unique_ptr<class VertexArray> -- SKIP this, not yet, Step 3
  - In constructor, after window creation: 
      m_TestShader = std::make_unique<Shader>(
          "assets/shaders/test.vert", "assets/shaders/test.frag");
  - Need ONE empty VAO bound for the draw call to be legal in Core Profile.
    Add private uint32_t m_EmptyVAO = 0; to Application, generate it via
    glGenVertexArrays(1, &m_EmptyVAO) in the constructor after the shader loads.
  - In destructor: glDeleteVertexArrays(1, &m_EmptyVAO);
  - In Run()'s loop, after glClear, before SwapBuffers:
      float time = (float)glfwGetTime();   // or std::chrono equivalent
      m_TestShader->Bind();
      m_TestShader->SetFloat("u_Time", time);
      glBindVertexArray(m_EmptyVAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);

━━━ MODIFY: CMakeLists.txt ─────────────────────────────────────────────────────

Add, right after target_include_directories:

# Copy assets folder next to the executable after every build, so relative
# shader paths like "assets/shaders/test.vert" resolve correctly at runtime
add_custom_command(TARGET Frostbytes POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Frostbytes>/assets"
)

━━━ EXPECTED OUTPUT ━━━

Same window as Step 1, but now showing a triangle with red/green/blue
vertex-interpolated corners, gently pulsing in brightness. Console log adds:
  [HH:MM:SS] [INFO]  Shader compiled and linked: assets/shaders/test.vert + assets/shaders/test.frag

If the shader fails to compile, the program should throw with the GLSL
compiler's actual error message visible in the console — not crash silently.

Zero compiler warnings. Zero GL errors (verify visually — no console spam,
clean triangle, no flicker/strobing).