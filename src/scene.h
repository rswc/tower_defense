#pragma once

#include <memory>
#include <vector>
#include "gameobject.h"
#include "camera.h"


class Scene
{
protected:
    std::vector<std::unique_ptr<GameObject>> objects;
    Camera activeCamera;
public:
    Scene() = default;
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = default;
    Scene& operator=(Scene&&) = default;
    ~Scene() = default;
    virtual void Update(double dt);
    virtual void Draw() const;
    void Instantiate(std::unique_ptr<GameObject> object);
};

