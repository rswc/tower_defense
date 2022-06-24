#pragma once

#include "gameobject.h"
#include "gamegrid.h"
#include "camera.h"

class GridObject : public GameObject {
public:
    GameGrid::GameGridMesh m_mesh;
    GridObject(const GameGrid::GameGridMesh &mesh) : m_mesh(mesh) {}
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime) override;
};
