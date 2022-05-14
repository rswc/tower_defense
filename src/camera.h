#pragma once

#include <glm/matrix.hpp>


class Camera
{
private:
    glm::mat4 v;
    glm::mat4 p;
public:
    Camera();
    glm::mat4 GetV() const;
    glm::mat4 GetP() const;
};

