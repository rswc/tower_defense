#pragma once

#include "gameobject.h"


class SillyAnimatedObject : public GameObject
{
private:
    float vertices[204][4]; //For now set by hand
    unsigned int vertexCount=204; //For now set by hand
public:
    SillyAnimatedObject();
    void Draw(const Camera& camera) const override;
};

