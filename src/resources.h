#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>

namespace Resources
{
    
    struct Character
    {
        glm::vec4 texCoords;
        glm::ivec2 size;
        glm::ivec2 bearing;
        long int advance;
    };

    struct Font
    {
        bool ready = false;
        unsigned int glyphHeight;
        GLuint texture;
        std::map<unsigned char, Character> characters;
    };

    extern Font ft_OpenSans;

    void Initialize();
    void Free();

} // namespace Resources


