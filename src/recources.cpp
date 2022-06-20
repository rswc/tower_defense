#include "resources.h"

#include <iostream>
#include <vector>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

namespace Resources
{
    Font ft_OpenSans;

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

    void Initialize() {
        ft_OpenSans = LoadFont("assets\\OpenSans\\OpenSans-Regular.ttf");
    }

    void Free() {
        glDeleteTextures(1, &ft_OpenSans.texture);
    }

} // namespace Resources

