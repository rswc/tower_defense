#pragma once

#include <memory>
#include "material.h"
#include "camera.h"

class GameObject
{
protected:
    std::shared_ptr<Material> material;
    // transform
    // ??
public:
    virtual void Update(double dt) {};
    virtual void Draw(const Camera& camera) const = 0;
};

