#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>


constexpr glm::vec4 Camera::FORWARD;
constexpr glm::vec3 Camera::UP;

Camera::Camera() {
    // placeholder
    // v = glm::lookAt(glm::vec3(0.0f, 1.5f, -1.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    p = glm::perspective(glm::radians(50.0f), 1.0f, 0.01f, 50.0f);
}

void Camera::SetScreenSize(float width, float height) {
    screenSize = glm::vec2(width, height);
}

glm::vec2 Camera::GetScreenSize() const {
    return screenSize;
}

glm::mat4 Camera::GetV() const {
    return glm::lookAt(
        GetPosition(),
        GetPosition() + glm::vec3(glm::mat4_cast(GetRotation()) * FORWARD),
        UP
    );
}

glm::mat4 Camera::GetP() const {
    return p;
}

glm::mat4 Camera::GetOrtho() const {
    return glm::ortho(0.0f, screenSize.x, 0.0f, screenSize.y);
}

