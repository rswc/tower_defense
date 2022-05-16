#pragma once



#include "gameobject.h"


typedef unsigned int GLuint;


class SillyAnimatedObject : public GameObject
{
private:
    float vertices[204][4]; //For now set by hand
    float texCoords[556][2]; //For now set by hand
    float colors[204][4]; //For now set by hand
    float normals[204][4]; //For now set by hand
    unsigned int vertexCount=204; //For now set by hand

    float angleY = 0.0f;

    GLuint tex; //Uchwyt

public:
    SillyAnimatedObject();
    void Update(double deltaTime)  override;
    void Draw(const Camera& camera) const override;
};

