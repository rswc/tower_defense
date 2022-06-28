#pragma once

#include <memory>
#include <vector>
#include <GLFW/glfw3.h>
#include "gameobject.h"
#include "rtscamera.h"

class Scene
{
  protected:
      std::vector<std::shared_ptr<GameObject>> objects;
      RTSCamera activeCamera;
  public:
    Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;
    ~Scene() = default;
    virtual void Update(double dt);
    virtual void Draw() const;
    virtual void OnKey(GLFWwindow* window, int key, int scancode, int action, int mod);
    virtual void OnMouse(GLFWwindow* window, double xpos, double ypos);
    virtual void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
    virtual void OnScroll(GLFWwindow* window, double xoffset, double yoffset);
    void Instantiate(std::shared_ptr<GameObject> object);
    void SetScreenSize(float width, float height);
    void UpdateDrawOrder();
};