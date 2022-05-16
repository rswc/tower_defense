#pragma once

#include "scene.h"


class SillyScene : public Scene
{
private:
    float speed_x, speed_y;
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

