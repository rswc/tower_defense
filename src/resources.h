#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>

class BaseMesh;
class AnimatedMesh;
class Animator;

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
    extern std::unordered_map<std::string, std::vector<BaseMesh>> bm_AssimpModels;
    extern std::unordered_map<std::string, GLuint> gl_Textures;
    
    const std::string MOB_TEXTURE = "mobtexture";
    const std::string TOWER_MODEL = "towermodel";
    const std::string TOWER_TEXTURE = "towertexture";

    BaseMesh* GetAssimpModelMesh(const std::string& key, int index);
    GLuint GetModelTexture(const std::string& key);

    GLuint LoadTexture(const char* filename);
    void Initialize();
    void Free();

    AnimatedMesh* GetMobAnimatedMesh(int index);
    Animator* GetNewMobAnimator();

} // namespace Resources

