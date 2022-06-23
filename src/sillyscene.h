#pragma once
#include <iostream>
#include "scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SillyScene : public Scene
{
private:
    float yaw = 0, pitch = 0, speed_fwd = 0, speed_right = 0;
    float lastX = 0, lastY = 0;
    float fov = 50.0f;
    bool firstMouse = true;
public:
    SillyScene();
    SillyScene(const SillyScene&) = delete;
    SillyScene& operator=(const SillyScene&) = delete;
    SillyScene(SillyScene&&) = default;
    SillyScene& operator=(SillyScene&&) = default;
    ~SillyScene() = default;
    void Update(double dt);
    void OnKey(GLFWwindow* window, int key, int scancode, int action, int mod) override;
    void OnMouse(GLFWwindow* window, double xpos, double ypos) override;
    void OnMouseButton(GLFWwindow* window, int button, int action, int mods) override;
    void OnScroll(GLFWwindow* window, double xoffset, double yoffset) override;
};

