#pragma once

#include "gameobject.h"
#include "gamegrid.h"
#include "camera.h"
#include "utility.h"

class GridObject : public GameObject {
private:
    GameGrid::GameGridMesh m_mesh;
    Plane m_plane;
public:
    GridObject(const GameGrid::GameGridMesh &mesh, Plane pickPlane) : m_mesh(mesh), m_plane(pickPlane) {}
    void Draw(const Camera& camera) const override;
    void Update(double deltaTime) override;
};
