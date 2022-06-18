#pragma once

#include "gameobject.h"
#include "BaseMesh.h"
#include "camera.h"

class GridObject : public GameObject {
public:
    BaseMesh m_mesh;
    GridObject(const BaseMesh &mesh) : m_mesh(mesh) {}
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime) override;
};
