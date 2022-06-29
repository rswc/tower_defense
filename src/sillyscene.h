#pragma once
#include <iostream>
#include "scene.h"
#include "gridobject.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gamegrid.h"
#include "mobmanager.h"

class SillyScene : public Scene
{
private:
    void SceneTransition() override;

    float yaw = -90, pitch = -45, speed_fwd = 0, speed_right = 0;
    float lastX = 0, lastY = 0;
    float fov = 50.0f;
    float cameraMoveSpeed = 2.5f;
    float  mouseSensitivity =  0.25f;
    glm::vec3 startCameraPosition = glm::vec3(0.0f, 12.0f, 3.0f);
    float cameraHeightCap = 3.0f;

    bool firstMouse = true, focus = true, freeFlight = false, doCameraRotation = false;
    void updateCursorState(GLFWwindow* window);
    std::shared_ptr<GridObject> gridObj;

    MobManager mobManager; 
    std::unique_ptr<GameGrid> grid;
public:
    SillyScene();
    SillyScene(const SillyScene&) = delete;
    SillyScene& operator=(const SillyScene&) = delete;
    SillyScene(SillyScene&&) = default;
    SillyScene& operator=(SillyScene&&) = default;
    ~SillyScene() = default;

    std::unique_ptr<Scene> GetTransitionTarget() const override;

    void Update(double dt);
    void OnKey(GLFWwindow* window, int key, int scancode, int action, int mod) override;
    void OnMouse(GLFWwindow* window, double xpos, double ypos) override;
    void OnMouseButton(GLFWwindow* window, int button, int action, int mods) override;
    void OnScroll(GLFWwindow* window, double xoffset, double yoffset) override;
};

