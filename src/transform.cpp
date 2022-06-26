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

void Transformable::CumulativeRotation(float angle, glm::vec3 axis){
    if(axis.x != 0.0f) rotX -= angle;
    if(axis.y != 0.0f) rotY += angle;
    
    //Clamp 6.28 rad is 1 full rotation
    if(rotX > 6.28318531f) rotX = rotX - 6.28318531f;
    if(rotY > 6.28318531f) rotY = rotY - 6.28318531f;
    if(rotX < -6.28318531f) rotX = rotX + 6.28318531f;
    if(rotY < -6.28318531f) rotY = rotY + 6.28318531f;

    transform.rotation = glm::rotate(glm::quat_cast(glm::mat4(1.0f)), rotX, glm::vec3(1,0,0));
    transform.rotation = glm::rotate(transform.rotation, rotY, glm::vec3(0,1,0));
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

float Transformable::GetDistanceTo(glm::vec3 point) const {
    return glm::distance(transform.position, point);
}

