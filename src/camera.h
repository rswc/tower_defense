#pragma once

#include <glm/matrix.hpp>
#include "transform.h"


class Camera : public Transformable
{
private:
    const glm::vec3 FORWARD = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 p;
public:
    Camera();
    glm::mat4 GetV() const;
    glm::mat4 GetP() const;
};

