#pragma once

#include "gameobject.h"

class Tower : public GameObject
{
private:
    // mesh...
    bool init = true;
public:

    void Draw(const Camera& camera) const override {};
    void Update(double deltaTime) override;
};

