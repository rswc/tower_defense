#include "resources.h"

#include <iostream>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

namespace Resources
{
    Font ft_OpenSans;

    Font LoadFont(const char* path) {
        Font font;

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

        FT_Set_Pixel_Sizes(face, 0, 48);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 32; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // now store character for later use
            Character character = {
                texture, 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };

            font.characters.insert(std::pair<char, Character>(c, character));
            
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    
        font.ready = true;

        return font;
    }

    void Initialize() {
        ft_OpenSans = LoadFont("assets\\OpenSans\\OpenSans-Regular.ttf");
    }

    void Free() {
        for (auto& entry : ft_OpenSans.characters) {
            glDeleteTextures(1, &entry.second.texture);
        }
    }

} // namespace Resources


