#pragma once

#include "gameobject.h"
#include "lodepng.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <iostream>

typedef unsigned int GLuint;
class AssimpObject : public GameObject
{
private:
    GLuint texture;
    std::vector<glm::vec4> verts;
    std::vector<glm::vec4> norms;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    void importMesh();
public:
    AssimpObject();
    ~AssimpObject();
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime)  override;
    void loadModel(std::string filename);
    GLuint readTexture(const char* filename);
};

