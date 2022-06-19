#pragma once

#include "gameobject.h"

#include <string>
#include <vector>


class Text : public GameObject
{
private:
    std::string text;
    float scale = 1.0f;
    std::vector<float> vertices;
    void updateMesh();
public:
    Text(std::string text);
    void SetText(std::string text);
    void SetScale(float scale);
    void Update(double dt) {};
    void Draw(const Camera& camera) const;
};

