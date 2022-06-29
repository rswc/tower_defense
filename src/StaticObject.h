#pragma once
#include "gameobject.h"
#include "BaseMesh.h"
#include "assimploader.h"

typedef unsigned int GLuint;
class StaticObject : public GameObject
{
private:

public:
    GLuint tex, texSpecular, texDiffuse;
    void importMesh();
    BaseMesh* mesh = nullptr;
    StaticObject(
        const std::string res_model,
        const std::string res_texture,
        const std::string res_specular
    );
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime)  override {}
    virtual void loadModel(std::string filename) {}
};
