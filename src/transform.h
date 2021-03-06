#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


struct Transform
{
    Transform(
        glm::vec3 pos = glm::vec3(0.0f),
        glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // w x y z for some reason
        glm::vec3 scale = glm::vec3(1.0f)
    ) : position(pos), rotation(rot), scale(scale) {};

    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    inline glm::mat4 GetMatrix() const {
        return glm::scale(
            glm::translate(glm::mat4(1.0), position) *
            glm::mat4_cast(rotation),
            scale
        );
    }
};

class Transformable
{
private:
    Transform transform;
public: // quickfix
    float rotX =0.0f, rotY=0.0f;
public:
    Transformable() : transform() {};
    void SetPosition(glm::vec3 pos);
    void SetRotation(glm::quat rot);
    void SetScale(glm::vec3 scale);
    void Rotate(float angle, glm::vec3 axis);
    void CumulativeRotation(float angle, glm::vec3 axis);
    glm::vec3 GetPosition() const;
    glm::quat GetRotation() const;
    glm::vec3 GetScale() const;
    glm::mat4 GetTransformMatrix() const;
    float GetDistanceTo(glm::vec3 point) const;
};

