#include "transform.h"


void Transformable::SetPosition(glm::vec3 pos) {
    transform.position = pos;
}

void Transformable::SetRotation(glm::quat rot) {
    transform.rotation = rot;
}

void Transformable::SetScale(glm::vec3 scale) {
    transform.scale = scale;
}

void Transformable::Rotate(float angle, glm::vec3 axis) {
    transform.rotation = glm::rotate(transform.rotation, angle, axis);
}

glm::vec3 Transformable::GetPosition() const {
    return transform.position;
}

glm::quat Transformable::GetRotation() const {
    return transform.rotation;
}

glm::vec3 Transformable::GetScale() const {
    return transform.scale;
}

glm::mat4 Transformable::GetTransformMatrix() const {
    return transform.GetMatrix();
}

