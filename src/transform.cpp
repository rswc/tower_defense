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

glm::vec3 Transformable::GetPosition() const {
    return transform.position;
}

glm::quat Transformable::GetRotation() const {
    return transform.rotation;
}

glm::vec3 Transformable::GetScale() const {
    return transform.scale;
}

