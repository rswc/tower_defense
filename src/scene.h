#pragma once

#include <memory>
#include <vector>
#include <GLFW/glfw3.h>
#include "gameobject.h"
#include "rtscamera.h"

class Scene
{
protected:
    enum DirtyFlag : short {
        DIRTY_DRAW_ORDER = 1
    };

    std::vector<std::shared_ptr<GameObject>> objects;
    RTSCamera activeCamera;
    bool transitionFlag = false;
    virtual void SceneTransition();
    short dirtyFlag = 0;
public:
    Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;
    ~Scene() = default;

    inline bool IsTransitionInitiated() const { return transitionFlag; };
    virtual std::unique_ptr<Scene> GetTransitionTarget() const = 0;

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