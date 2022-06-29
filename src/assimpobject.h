#pragma once

#include "BaseMesh.h"
#include "gameobject.h"
#include "lodepng.h"
#include "assimploader.h"
#include <vector>
#include <iostream>

typedef unsigned int GLuint;
class AssimpObject : public GameObject
{
private:

public:
    GLuint texture, textSpecular, textDiffuse;
    void importMesh();
    std::vector<BaseMesh> meshes;
    AssimpObject();
    ~AssimpObject();
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime)  override;
    virtual void loadModel(std::string filename) {};
    GLuint readTexture(const char* filename);
};

