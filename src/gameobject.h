#pragma once

#include <memory>
#include "transform.h"
#include "camera.h"


class GameObject : public Transformable
{
protected:
    // ??
public:
    virtual void Update(double dt) = 0;
    virtual void Draw(const Camera& camera) const = 0;
    virtual float GetOrder(const Camera& camera) const;
};

