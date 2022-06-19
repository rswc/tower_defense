#pragma once

#include <glm/matrix.hpp>
#include "transform.h"


class Camera : public Transformable
{
private:
    constexpr static glm::vec4 FORWARD = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    constexpr static glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 p;
public:
    Camera();
    glm::mat4 GetV() const;
    glm::mat4 GetP() const;
    glm::vec3 GetForward() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetRight() const;
};

