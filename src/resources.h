#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>

namespace Resources
{
    
    struct TextureArray
    {
        GLuint id;
        unsigned int width, height, layers;
    };

    extern TextureArray ta_Terrain;

    void Initialize();
    void Free();

} // namespace Resources

