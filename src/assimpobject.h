#pragma once

#include "BaseMesh.h"
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
    void importMesh();
public:
    BaseMesh objectMesh;
    AssimpObject();
    ~AssimpObject();
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime)  override;
    void loadModel(std::string filename);
    GLuint readTexture(const char* filename);
};

