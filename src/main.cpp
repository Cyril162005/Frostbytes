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
