#pragma once
#include <string>
#include <memory>
#include "3d/Mesh.h"

namespace fb {

class OBJLoader {
public:
    static std::shared_ptr<Mesh> Load(const std::string& path);
};

} // namespace fb
