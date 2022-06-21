#include "resources.h"

#include <iostream>
#include <vector>
#include <string>
#include "lodepng.h"


namespace Resources
{
    TextureArray ta_Terrain;

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

    void Initialize() {
        ta_Terrain = LoadTextureArray({"assets/ta_Test_0.png", "assets/ta_Test_1.png"});
    }

    void Free() {
        glDeleteTextures(1, &ta_Terrain.id);
    }

} // namespace Resources
