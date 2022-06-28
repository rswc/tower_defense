#pragma once

#include <GL/glew.h>
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

    struct TextureArray
    {
        GLuint id;
        unsigned int width, height, layers;
    };

    struct Cubemap
    {
        GLuint id;
    };

    extern Font ft_OpenSans;
    extern TextureArray ta_Terrain;
    extern Cubemap cmp_Skybox;

    void Initialize();
    void Free();

} // namespace Resources

