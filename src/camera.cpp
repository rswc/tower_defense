#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() {
    // placeholder
    v = glm::lookAt(glm::vec3(0.0f, 1.5f, -1.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    p = glm::perspective(glm::radians(50.0f), 1.0f, 0.01f, 50.0f);
}

glm::mat4 Camera::GetV() const {
    return v;
}

glm::mat4 Camera::GetP() const {
    return p;
}

