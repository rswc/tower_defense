#include "resources.h"

#include "lodepng.h"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "assimploader.h"
#include "assimploaderanimated.h"
#include "Animator.h" 

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Resources
{
    Font ft_OpenSans;
    TextureArray ta_Terrain;
    Cubemap cmp_Skybox;
    std::unordered_map<std::string, std::vector<BaseMesh>> bm_AssimpModels;
    std::unordered_map<std::string, GLuint> gl_Textures;
    
    std::vector<std::unique_ptr<Animation>> mobobjectAnimations;
    std::vector<std::unique_ptr<Animator>> mobobjectAnimators;
    std::vector<AnimatedMesh> mobobjectAnimatedMeshes;

    AnimatedAssimpLoader animatedLoader;
    Assimp::Importer mobobjectImporter;
    const aiScene* mobobjectScene = nullptr;
    
    // Adapted from https://learnopengl.com/In-Practice/Text-Rendering
    Font LoadFont(const char* path, FT_UInt glyphHeight = 48) {
        Font font;
        font.glyphHeight = glyphHeight;

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return font;
        }

        FT_Face face;
        if (FT_New_Face(ft, path, 0, &face))
        {
            std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;  
            return font;
        }

        FT_Set_Pixel_Sizes(face, 0, glyphHeight);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        int combinedWidth = 0, combinedHeight = 0;
        for (unsigned char c = 32; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            if (face->glyph->bitmap.rows > combinedHeight)
                combinedHeight = face->glyph->bitmap.rows;
            
            combinedWidth += face->glyph->bitmap.width;

        }

        GLint maxSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
        if (combinedWidth > maxSize)
        {
            std::cerr << "WARNING: This system can't suppport textures large enough for this font!" << std::endl;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        font.texture = texture;

        glBindTexture(GL_TEXTURE_2D, texture);

        std::vector<GLubyte> emptyData(combinedWidth * combinedHeight, 0);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            combinedWidth,
            combinedHeight,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            &emptyData[0]
        );
        
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int curOffset = 0;

        for (unsigned char c = 32; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            glTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                curOffset,
                0,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            
            // now store character for later use
            Character character = {
                glm::vec4(
                    static_cast<float>(curOffset) / combinedWidth,
                    static_cast<float>(curOffset + face->glyph->bitmap.width) / combinedWidth,
                    0,
                    static_cast<float>(face->glyph->bitmap.rows) / combinedHeight
                ), 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };

            curOffset += face->glyph->bitmap.width;

            font.characters.insert(std::pair<char, Character>(c, character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    
        font.ready = true;

        return font;
    }

    TextureArray LoadTextureArray(std::vector<std::string> images) {
        TextureArray ta;

        if (images.size() < 1)
        {
            std::cerr << "ERROR: TextureArray: Too few images.\n";
            return ta;
        }

        ta.layers = images.size();

        glActiveTexture(GL_TEXTURE0);
        std::vector<unsigned char> image;
        unsigned width, height;

        unsigned error = lodepng::decode(image, width, height, images[0].c_str());
        if (error != 0)
        {
            std::cerr << "ERROR: Lodepng: " << lodepng_error_text(error) << std::endl;
            return ta;
        }


        ta.width = width;
        ta.height = height;

        glGenTextures(1, &ta.id);
        glBindTexture(GL_TEXTURE_2D_ARRAY, ta.id);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, ta.width, ta.height, ta.layers);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            0,
            0,
            0,
            ta.width,
            ta.height,
            1,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            (unsigned char*)image.data()
        );

        for (int layer = 1; layer < images.size(); ++layer)
        {
            image.clear();
            std::cout << "Loading " << images[layer].c_str() << "now...\n";
            unsigned error = lodepng::decode(image, width, height, images[layer].c_str());
            if (error != 0)
            {
                std::cerr << "ERROR: Lodepng: " << lodepng_error_text(error) << std::endl;
                return ta;
            }
            if (width != ta.width || height != ta.height)
            {
                std::cerr << "ERROR: TextureArray: Image dimension mismatch!";
                return ta;
            }
            
            glTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                0,
                0,
                0,
                layer,
                ta.width,
                ta.height,
                1,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                (unsigned char*)image.data()
            );
        }
        
        return ta;
    }

    Cubemap LoadCubemap(std::vector<std::string> images) {
        Cubemap cmp;
        glGenTextures(1, &cmp.id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cmp.id);
        glActiveTexture(GL_TEXTURE0);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        for (int i = 0; i < images.size(); i++)
        {
            std::vector<unsigned char> image;
            unsigned width, height;

            unsigned error = lodepng::decode(image, width, height, images[i].c_str());
            if (error != 0) {
                std::cerr << "ERROR: Lodepng: " << lodepng_error_text(error) << std::endl;
                return cmp;
            } else {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB,
                    width,
                    height,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    image.data() 
                );
            }
        }

        return cmp;
    }

    GLuint LoadTexture(const char* filename) {	
      GLuint tex;	
      glActiveTexture(GL_TEXTURE0); 	

      //Wczytanie do pamięci komputera
      std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
      unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
      //Wczytaj obrazek
      unsigned error = lodepng::decode(image, width, height, filename);
     
      //Import do pamięci karty graficznej
      glGenTextures(1,&tex); //Zainicjuj jeden uchwyt
      glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
      //Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
      glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*) image.data());	

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

      return tex;
    } 

    void Initialize() {
        ft_OpenSans = LoadFont("assets\\OpenSans\\OpenSans-Regular.ttf");
        ta_Terrain = LoadTextureArray({"assets\\ta_Terrain_0.png", "assets\\ta_Terrain_1.png"});
        cmp_Skybox = LoadCubemap({
            "assets\\envmap_stormydays\\stormydays_ft.png",
            "assets\\envmap_stormydays\\stormydays_bk.png",
            "assets\\envmap_stormydays\\stormydays_up.png",
            "assets\\envmap_stormydays\\stormydays_dn.png",
            "assets\\envmap_stormydays\\stormydays_rt.png",
            "assets\\envmap_stormydays\\stormydays_lf.png",
        });
        auto assimpLoader = AssimpLoader();
        assimpLoader.loadModel("assets/tower.blend");
        bm_AssimpModels[TOWER_MODEL] = assimpLoader.getMeshes();
        gl_Textures[TOWER_TEXTURE] = LoadTexture("assets/tower_diffuse.tga");
        gl_Textures[MOB_TEXTURE] = LoadTexture("assets/trex/trex.png");
        // gl_Textures[MOBOBJECT_TEXTURE_SPECULAR] = LoadTexture("assets/trex/trex_specular.png");
        
        animatedLoader = AnimatedAssimpLoader();
        std::string mob_anim_model = "assets/trex/trex.glb";
        animatedLoader.loadModel(mob_anim_model);
        mobobjectAnimatedMeshes = animatedLoader.getMeshes();

        mobobjectScene = mobobjectImporter.ReadFile(mob_anim_model, aiProcess_Triangulate);
    }
    
    BaseMesh* GetAssimpModelMesh(const std::string& key, int index) {
        try {
            std::vector<BaseMesh>& ref = bm_AssimpModels.at(key);
            if ((unsigned) index < ref.size()) {
                return &ref[index];
            }
            else return nullptr;
        } catch (const std::out_of_range& e) {
            return nullptr;
        }
    } 

    AnimatedMesh* GetMobAnimatedMesh(int index) {
        return &mobobjectAnimatedMeshes[index];
    }
    
    Animator* GetNewMobAnimator() {
        std::cout << std::hex << mobobjectScene << " Maybe here?" << std::endl;
        std::cout << "Starting Animation contructor" << std::endl;
        auto new_anim = std::make_unique<Animation>(mobobjectScene, &animatedLoader);
        std::cout << "Starting Animator contructor" << std::endl;
        auto new_animator = std::make_unique<Animator>(new_anim.get());
        std::cout << "Starting End contructor" << std::endl;
        mobobjectAnimations.push_back(std::move(new_anim));
        mobobjectAnimators.push_back(std::move(new_animator));
        return mobobjectAnimators.back().get();
    }

    GLuint GetModelTexture(const std::string& key) {
        try {
            return gl_Textures.at(key);
        } catch (const std::out_of_range& e) {
            return 0;
        }
    }

    void Free() {
        glDeleteTextures(1, &ft_OpenSans.texture);
        glDeleteTextures(1, &ta_Terrain.id);
        glDeleteTextures(1, &cmp_Skybox.id);
        for (auto& p : gl_Textures) {
            glDeleteTextures(1, &p.second);
        } 
    }

} // namespace Resources
