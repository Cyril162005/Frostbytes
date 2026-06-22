#pragma once
#include <glm/glm.hpp>
#include <cstdint>

namespace fb {

class Input {
public:
    static bool IsKeyPressed(int keycode);
    static bool IsMouseButtonPressed(int button);
    static glm::vec2 GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};

} // namespace fb
