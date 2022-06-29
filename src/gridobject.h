#pragma once

#include "gameobject.h"
#include "gamegrid.h"
#include "camera.h"
#include "utility.h"

class GridObject : public GameObject {
private:
    Grid m_grid;
    GameGrid::GameGridMesh m_mesh;
    Plane m_plane;
public:
    GridObject(
        const GameGrid::GameGridMesh &mesh,
        const Plane& pickPlane,
        const Grid& logicalGrid
    ) : m_mesh(mesh), m_plane(pickPlane), m_grid(logicalGrid) {}

    void Draw(const Camera& camera) const override;
    void Update(double deltaTime) override;
    Grid& GetLogical();
    Plane GetMousePickPlane() const;
    Grid::GridPosition WorldToGrid(glm::vec3 position) const;
};
