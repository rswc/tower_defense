#pragma once

#include <memory>
#include "material.h"
#include "transform.h"
#include "camera.h"


class GameObject : public Transformable
{
protected:
    std::shared_ptr<Material> material;
    // ??
public:
    virtual void Update(double dt) {};
    virtual void Draw(const Camera& camera) const = 0;
};

