#pragma once

#include "BaseMesh.h"
#include "gameobject.h"
#include "lodepng.h"
#include "gamegrid.h"
#include <vector>
#include <iostream>

typedef unsigned int GLuint;
class MobObject : public GameObject
{
private:
    GLuint texture, textSpecular, textDiffuse;
    void importMesh();
    GameGrid * gameGrid;
public:
    std::vector<BaseMesh> meshes;
    MobObject(GameGrid * gameGridPointer);
    ~MobObject();
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime)  override;
    void loadModel(std::string filename);
    GLuint readTexture(const char* filename);
};

