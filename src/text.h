#pragma once

#include "gameobject.h"

#include <string>
#include <vector>


class Text : public GameObject
{
private:
    glm::vec2 origin;
    std::string text;
    float scale = 1.0f;
    glm::vec4 color = glm::vec4(1.0f);
    std::vector<float> vertices;
    void updateMesh();
public:
    Text(std::string text);
    void SetOrigin(glm::vec2 pos);
    void SetText(std::string text);
    void SetScale(float scale);
    void SetColor(glm::vec4 color);
    void Update(double dt) {};
    void Draw(const Camera& camera) const;
};

