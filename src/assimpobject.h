#pragma once

#include "BaseMesh.h"
#include "gameobject.h"
#include "lodepng.h"
#include <vector>
#include <iostream>

typedef unsigned int GLuint;
class AssimpObject : public GameObject
{
private:
    GLuint texture, textSpecular, textDiffuse;
    void importMesh();
public:
    std::vector<BaseMesh> meshes;
    AssimpObject();
    ~AssimpObject();
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime)  override;
    void loadModel(std::string filename);
    GLuint readTexture(const char* filename);
};

