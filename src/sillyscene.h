#pragma once
#include <iostream>
#include "scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SillyScene : public Scene
{
private:
    float speed_yaw = 0, speed_pitch = 0, speed_fwd = 0, speed_right = 0;
public:
    SillyScene();
    SillyScene(const SillyScene&) = delete;
    SillyScene& operator=(const SillyScene&) = delete;
    SillyScene(SillyScene&&) = default;
    SillyScene& operator=(SillyScene&&) = default;
    ~SillyScene() = default;
    void Update(double dt);
    void OnKey(GLFWwindow* window, int key, int scancode, int action, int mod) override;
};

