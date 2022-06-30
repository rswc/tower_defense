#pragma once
#include "gameobject.h"
#include "BaseMesh.h"
#include "assimploader.h"

typedef unsigned int GLuint;
class StaticObject : public GameObject
{
private:

public:
    GLuint texture, textSpecular, textDiffuse;
    void importMesh();
    std::vector<BaseMesh> meshes;
    StaticObject(
        const std::string res_model,
        const std::string res_texture,
        const std::string res_specular
    );
    ~StaticObject();
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime)  override {}
    virtual void loadModel(std::string filename) {}
};
