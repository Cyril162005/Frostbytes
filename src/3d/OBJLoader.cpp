#include "OBJLoader.h"
#include "core/Logger.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

namespace fb {

struct OBJVertexIndex {
    int p, n, t;

    bool operator==(const OBJVertexIndex& other) const {
        return p == other.p && n == other.n && t == other.t;
    }
};

struct OBJVertexIndexHash {
    size_t operator()(const OBJVertexIndex& v) const {
        return ((std::hash<int>()(v.p) ^ (std::hash<int>()(v.n) << 1)) >> 1) ^ (std::hash<int>()(v.t) << 1);
    }
};

std::shared_ptr<Mesh> OBJLoader::Load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        FB_ERROR("Failed to open OBJ file: " + path);
        return nullptr;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    std::vector<Vertex>   vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<OBJVertexIndex, uint32_t, OBJVertexIndexHash> vertexCache;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 p;
            ss >> p.x >> p.y >> p.z;
            positions.push_back(p);
        } else if (prefix == "vn") {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        } else if (prefix == "vt") {
            glm::vec2 t;
            ss >> t.x >> t.y;
            texCoords.push_back(t);
        } else if (prefix == "f") {
            std::string vertexStr;
            while (ss >> vertexStr) {
                // Format: p/t/n
                OBJVertexIndex vi = { 0, 0, 0 };
                size_t firstSlash = vertexStr.find('/');
                size_t lastSlash = vertexStr.find_last_of('/');

                vi.p = std::stoi(vertexStr.substr(0, firstSlash));
                
                if (firstSlash != std::string::npos && firstSlash != lastSlash) {
                    // Has t and n (p/t/n)
                    std::string tStr = vertexStr.substr(firstSlash + 1, lastSlash - firstSlash - 1);
                    if (!tStr.empty()) vi.t = std::stoi(tStr);
                    vi.n = std::stoi(vertexStr.substr(lastSlash + 1));
                } else if (firstSlash != std::string::npos) {
                    // Has either t or n (p/t or p//n)
                    if (vertexStr[firstSlash + 1] == '/') {
                        vi.n = std::stoi(vertexStr.substr(firstSlash + 2));
                    } else {
                        vi.t = std::stoi(vertexStr.substr(firstSlash + 1));
                    }
                }

                // OBJ indices are 1-based, convert to 0-based
                vi.p -= 1;
                if (vi.t > 0) vi.t -= 1; else vi.t = -1;
                if (vi.n > 0) vi.n -= 1; else vi.n = -1;

                if (vertexCache.count(vi) == 0) {
                    Vertex v;
                    v.Position = positions[vi.p];
                    v.Normal   = (vi.n >= 0) ? normals[vi.n]   : glm::vec3(0.0f);
                    v.TexCoord = (vi.t >= 0) ? texCoords[vi.t] : glm::vec2(0.0f);
                    
                    vertexCache[vi] = (uint32_t)vertices.size();
                    vertices.push_back(v);
                }
                indices.push_back(vertexCache[vi]);
            }
        }
    }

    FB_INFO("OBJ Loaded: " + path + " (" + std::to_string(vertices.size()) + " vertices, " + std::to_string(indices.size() / 3) + " faces)");
    return std::make_shared<Mesh>(vertices, indices);
}

} // namespace fb
