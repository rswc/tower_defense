#pragma once
#include <vector>
#include <glm/glm.hpp>

class BaseMesh
{
    public:
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec4> normals;
        std::vector<glm::vec2> textures;
        std::vector<unsigned int> indices;
};