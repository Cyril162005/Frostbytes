#include "Input.h"
#include "core/Application.h"
#include "platform/Window.h"
#include <GLFW/glfw3.h>

namespace fb {

bool Input::IsKeyPressed(int keycode) {
    auto window = Application::Get().GetWindow().GetNativeWindow();
    int state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button) {
    auto window = Application::Get().GetWindow().GetNativeWindow();
    int state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition() {
    auto window = Application::Get().GetWindow().GetNativeWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return { (float)xpos, (float)ypos };
}

float Input::GetMouseX() {
    return GetMousePosition().x;
}

float Input::GetMouseY() {
    return GetMousePosition().y;
}

} // namespace fb
