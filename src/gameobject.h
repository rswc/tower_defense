#pragma once

#include <memory>
#include "transform.h"
#include "camera.h"


class GameObject : public Transformable
{
protected:
    // ??
public:
    enum DrawLayer : int {
        LAYER_UI,
        LAYER_TRANSPARENT,
        LAYER_SKYBOX,
        LAYER_OPAQUE
    };

    struct DrawOrderPosition
    {
        DrawLayer layer;
        float depth;

        bool operator>(const DrawOrderPosition& other) const {
            if (layer == other.layer)
            {
                return depth > other.depth;
            }
            return layer > other.layer;
        }
    };

    virtual void Update(double dt) = 0;
    virtual void Draw(const Camera& camera) const = 0;
    virtual DrawOrderPosition GetOrder(const Camera& camera) const;
};

