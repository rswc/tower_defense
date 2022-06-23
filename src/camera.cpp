#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>


constexpr glm::vec4 Camera::FORWARD;
constexpr glm::vec3 Camera::UP;
float radius = 3.0f;

glm::vec3 cameraDirection; 

Camera::Camera() {
    // placeholder
    // v = glm::lookAt(glm::vec3(0.0f, 1.5f, -1.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    p = glm::perspective(glm::radians(50.0f), 1.0f, 0.01f, 50.0f);

    cameraDirection = glm::normalize(GetPosition() - glm::vec3(0.0f, 0.0f, 0.0f));
}

glm::mat4 Camera::GetV() const {
    return glm::lookAt(
        GetPosition(),
        // GetPosition() + glm::vec3(GetForward()),
        cameraDirection,
        UP
    );
}

void Camera::setLookAt(float camX, float camZ){
    glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Camera::GetP() const {
    return p;
}

glm::vec3 Camera::GetForward() const {
    return glm::mat4_cast(GetRotation()) * FORWARD;
}

glm::vec3 Camera::GetUp() const {
   // return glm::mat4_cast(GetRotation()) * glm::vec4(UP, 0.0f);
   return glm::cross(cameraDirection, GetRight());
}

glm::vec3 Camera::GetRight() const {
    //return glm::cross(glm::vec3(GetForward()), glm::vec3(GetUp()));
    return glm::normalize(glm::cross(UP, cameraDirection));
}


