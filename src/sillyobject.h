#pragma once

#include "gameobject.h"


class SillyObject : public GameObject
{
private:

public:
    SillyObject();
    void Draw(const Camera& camera) const override;
};

