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
    int currentMap;

    static const int numAvailableMaps = 4;

    float yaw = -90, pitch = -45, speed_fwd = 0, speed_right = 0;
    float lastX = 0, lastY = 0;
    float fov = 50.0f;

    bool firstMouse = true, focus = true, freeFlight = false, doCameraRotation = false;
    void updateCursorState(GLFWwindow* window);
    std::shared_ptr<GridObject> gridObj;

    MobManager mobManager; 
    std::unique_ptr<GameGrid> grid;
public:
    SillyScene(int mapID = 0);
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

