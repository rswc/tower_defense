#include "gamegrid.h"

#include "utility.h"
#include "grid.h"
#include "BaseMesh.h"
#include "gridobject.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

GameGrid::GameGridPosition GameGrid::gridToModelPosition(Grid::GridPosition p) const {
    if (!logicalGrid.isInsideGrid(p)) {
        return {-1, -1, -1};
    }
    
    return { p.row + halfRowScale, cellElevation(p), p.col + halfColScale };
}

inline float GameGrid::cellElevation(Grid::GridPosition p) const {
    if (logicalGrid.isInsideGrid(p))
        return isFlat(p) ? elevationStep : 0;
    return elevationStep;
}

float GameGrid::cornerElevation(Grid::GridPosition a, Grid::GridPosition b, Grid::GridPosition c) {
    float aEl = cellElevation(a);
    float bEl = cellElevation(b);
    float cEl = cellElevation(c);

    float t = (aEl > bEl) ? aEl : bEl;
    return (cEl > t) ? cEl : t;
}
    
void GameGrid::appendTriangle(std::vector<glm::vec4>& vertexArray, GameGrid::GameGridPosition a, 
    GameGrid::GameGridPosition b, GameGrid::GameGridPosition c) {
    vertexArray.emplace_back(a, 1.f);
    vertexArray.emplace_back(b, 1.f);
    vertexArray.emplace_back(c, 1.f);
}
    
void GameGrid::appendTriangle(std::vector<glm::vec3>& vertexArray, GameGridPosition a, GameGridPosition b, GameGridPosition c) {
    vertexArray.emplace_back(a);
    vertexArray.emplace_back(b);
    vertexArray.emplace_back(c);
}

void GameGrid::appendRectangle(
    GameGridMesh& mesh,
    GameGridPosition upperRight,
    GameGridPosition lowerRight,
    GameGridPosition upperLeft,
    GameGridPosition lowerLeft
) {
    // Upper-Right triangle 
    appendTriangle(mesh.vertices, upperLeft, lowerRight, upperRight);
    appendTriangle(mesh.normals, 
        calculateTriangleNormal(upperLeft, lowerRight, upperRight),
        calculateTriangleNormal(lowerRight, upperRight, upperLeft),
        calculateTriangleNormal(upperRight, upperLeft, lowerRight));
    
    // HACK: This could break very easily, I think
    appendTriangle(
        mesh.textures,
        {0.f, 1.f, (upperLeft.y > 0.1f) * 1.f},
        {0.f, 1.f, (lowerRight.y > 0.1f) * 1.f},
        {0.f, 1.f, (upperRight.y > 0.1f) * 1.f}
    );

    // Lower-Left triangle
    appendTriangle(mesh.vertices, upperLeft, lowerLeft, lowerRight);
    appendTriangle(mesh.normals, 
        calculateTriangleNormal(upperLeft, lowerLeft, lowerRight),
        calculateTriangleNormal(lowerLeft, lowerRight, upperLeft),
        calculateTriangleNormal(lowerRight, upperLeft, lowerLeft));
    appendTriangle(
        mesh.textures,
        {0.f, 1.f, (upperLeft.y > 0.1f) * 1.f},
        {0.f, 1.f, (lowerLeft.y > 0.1f) * 1.f},
        {0.f, 1.f, (lowerRight.y > 0.1f) * 1.f}
    );
}

void GameGrid::makeFlatCell(GameGridMesh& mesh, int row, int column) {
    GameGridPosition cellCenter = gridToModelPosition({row, column});
    
    appendRectangle(
        mesh,
        cellCenter + dirVector[GAME_DIR_UPRIGHT],
        cellCenter + dirVector[GAME_DIR_DOWNRIGHT],
        cellCenter + dirVector[GAME_DIR_UPLEFT],
        cellCenter + dirVector[GAME_DIR_DOWNLEFT]
    );
}

void GameGrid::makeCanyonCell(GameGridMesh& mesh, int row, int column) {
    GameGridPosition cellCenter = gridToModelPosition({row, column});

    GameGridPosition innerLowerRight = cellCenter + innerSquareFactor * dirVector[GAME_DIR_DOWNRIGHT];
    GameGridPosition innerUpperRight = cellCenter + innerSquareFactor * dirVector[GAME_DIR_UPRIGHT];
    GameGridPosition innerLowerLeft = cellCenter + innerSquareFactor * dirVector[GAME_DIR_DOWNLEFT];
    GameGridPosition innerUpperLeft = cellCenter + innerSquareFactor * dirVector[GAME_DIR_UPLEFT];

    GameGridPosition outerLowerRight = cellCenter + dirVector[GAME_DIR_DOWNRIGHT];
    GameGridPosition outerUpperRight = cellCenter + dirVector[GAME_DIR_UPRIGHT];
    GameGridPosition outerLowerLeft = cellCenter + dirVector[GAME_DIR_DOWNLEFT];
    GameGridPosition outerUpperLeft = cellCenter + dirVector[GAME_DIR_UPLEFT];

    // Blessh this mesh
    
    // Inner square
    appendRectangle(
        mesh,
        innerUpperRight,
        innerLowerRight,
        innerUpperLeft,
        innerLowerLeft
    );

    // Upper edge strip
    appendRectangle(
        mesh,
        { outerUpperRight.x, cellElevation({row - 1, column}), innerUpperRight.z },
        innerUpperRight,
        { outerUpperLeft.x, cellElevation({row - 1, column}), innerUpperLeft.z },
        innerUpperLeft
    );

    // Right edge strip
    appendRectangle(
        mesh,
        { innerLowerRight.x, cellElevation({row, column + 1}), outerLowerRight.z },
        innerLowerRight,
        { innerUpperRight.x, cellElevation({row, column + 1}), outerUpperRight.z },
        innerUpperRight
    );

    // Lower edge strip
    appendRectangle(
        mesh,
        { outerLowerLeft.x, cellElevation({row + 1, column}), innerLowerLeft.z },
        innerLowerLeft,
        { outerLowerRight.x, cellElevation({row + 1, column}), innerLowerRight.z },
        innerLowerRight
    );

    // Left edge strip
    appendRectangle(
        mesh,
        { innerUpperLeft.x, cellElevation({row, column - 1}), outerUpperLeft.z },
        innerUpperLeft,
        { innerLowerLeft.x, cellElevation({row, column - 1}), outerLowerLeft.z },
        innerLowerLeft
    );

    // Upper-right corner
    appendRectangle(
        mesh,
        { innerUpperRight.x, cellElevation({row, column + 1}), outerUpperRight.z },
        innerUpperRight,
        { outerUpperRight.x, cornerElevation({row, column + 1}, {row - 1, column}, {row - 1, column + 1}), outerUpperRight.z },
        { outerUpperRight.x, cellElevation({row - 1, column}), innerUpperRight.z }
    );

    // Lower-right corner
    appendRectangle(
        mesh,
        { innerLowerRight.x, cellElevation({row, column + 1}), outerLowerRight.z },
        { outerLowerRight.x, cornerElevation({row, column + 1}, {row + 1, column}, {row + 1, column + 1}), outerLowerRight.z },
        innerLowerRight,
        { outerLowerRight.x, cellElevation({row + 1, column}), innerLowerRight.z }
    );

    // Upper-left corner
    appendRectangle(
        mesh,
        { outerUpperLeft.x, cellElevation({row - 1, column}), innerUpperLeft.z },
        innerUpperLeft,
        { outerUpperLeft.x, cornerElevation({row, column - 1}, {row - 1, column}, {row - 1, column - 1}), outerUpperLeft.z },
        { innerUpperLeft.x, cellElevation({row, column - 1}), outerUpperLeft.z }
    );

    // Lower-left corner
    appendRectangle(
        mesh,
        { outerLowerLeft.x, cellElevation({row + 1, column}), innerLowerLeft.z },
        { outerLowerLeft.x, cornerElevation({row, column - 1}, {row + 1, column}, {row + 1, column - 1}), outerLowerLeft.z },
        innerLowerLeft,
        { innerLowerLeft.x, cellElevation({row, column - 1}), outerLowerLeft.z }
    );
}

void GameGrid::makeGridBorder(GameGridMesh& mesh, int rows, int columns) {
    GameGridPosition innerLowerRight = gridToModelPosition({rows - 1, columns - 1}) + dirVector[GAME_DIR_DOWNRIGHT];
    GameGridPosition innerUpperRight = gridToModelPosition({0, columns - 1}) + dirVector[GAME_DIR_UPRIGHT];
    GameGridPosition innerLowerLeft = gridToModelPosition({rows - 1, 0}) + dirVector[GAME_DIR_DOWNLEFT];
    GameGridPosition innerUpperLeft = gridToModelPosition({0, 0}) + dirVector[GAME_DIR_UPLEFT];

    GameGridPosition outerLowerRight = innerLowerRight + borderExtrusionParallel * dirVector[GAME_DIR_DOWNRIGHT]
        + borderExtrusionPerpendicular * depthVector;
    GameGridPosition outerUpperRight = innerUpperRight + borderExtrusionParallel * dirVector[GAME_DIR_UPRIGHT]
        + borderExtrusionPerpendicular * depthVector;
    GameGridPosition outerLowerLeft = innerLowerLeft + borderExtrusionParallel * dirVector[GAME_DIR_DOWNLEFT]
        + borderExtrusionPerpendicular * depthVector;
    GameGridPosition outerUpperLeft = innerUpperLeft + borderExtrusionParallel * dirVector[GAME_DIR_UPLEFT]
        + borderExtrusionPerpendicular * depthVector;

    // Lower edge
    appendRectangle(
        mesh,
        innerLowerRight,
        outerLowerRight,
        innerLowerLeft,
        outerLowerLeft
    );

    // Right edge
    appendRectangle(
        mesh,
        innerUpperRight,
        outerUpperRight,
        innerLowerRight,
        outerLowerRight
    );

    // Upper edge
    appendRectangle(
        mesh,
        innerUpperLeft,
        outerUpperLeft,
        innerUpperRight,
        outerUpperRight
    );

    // Left edge
    appendRectangle(
        mesh,
        innerLowerLeft,
        outerLowerLeft,
        innerUpperLeft,
        outerUpperLeft
    );
}
    
GameGrid::GameGridMesh GameGrid::generateSimpleMesh() {
    GameGridMesh mesh;
    
    for (int r = 0; r < logicalGrid.m_rows; r++) 
    for (int c = 0; c < logicalGrid.m_cols; c++) {
        makeFlatCell(mesh, r, c);
    }
    return mesh;
}

GameGrid::GameGridMesh GameGrid::generateWalledMesh() {
    GameGridMesh mesh;
    
    for (int r = 0; r < logicalGrid.m_rows; r++) 
    for (int c = 0; c < logicalGrid.m_cols; c++) {
        if (isFlat({r, c}))
        {
            makeFlatCell(mesh, r, c);
        } else {
            makeCanyonCell(mesh, r, c);
        }
    }

    makeGridBorder(mesh, logicalGrid.m_rows, logicalGrid.m_cols);

    return mesh;
}
    
GameGrid::GameGridMesh GameGrid::generateBaseMesh(GameGrid::MeshVersion version) {
    GameGridMesh mesh;

    switch (version) {
        case MESH_V_FIRST: mesh = generateSimpleMesh(); break;
        case MESH_V_SECOND: mesh = generateWalledMesh(); break;
    }
    
    return mesh;
}

GameGrid::GamePath GameGrid::generateGamePath(const Grid& grid) {
    GamePath path;
    
    GameDirEnum last_dir = GAME_DIR_NONE;
    GameDirEnum next_dir = GAME_DIR_NONE;
    for (auto &p : grid.m_path) {
        Grid::GridPosition logical_pos = p.first;
        Grid::DirEnum logical_dir = p.second;

        GameGridPosition center = gridToModelPosition(logical_pos);
        next_dir = (logical_dir == Grid::DIRECTION_NONE ? GAME_DIR_NONE : logical_dir * 2);

        if (last_dir == GAME_DIR_NONE)  {
            GameDirEnum opp_dir = grid.oppositeDirection(logical_dir) * 2;
            path.emplace_back(center + dirVector[opp_dir]);
        }

        path.emplace_back(center);
        
        if (next_dir == GAME_DIR_NONE) { 
            path.emplace_back(center + dirVector[last_dir]);
        }
        else {
            path.emplace_back(center + dirVector[next_dir]);
        }
        
        last_dir = next_dir;
    }

    return path;
}
Plane GameGrid::GetMousePickPlane() const {
    return Plane(
        gridToModelPosition({0, 0}),
        glm::normalize(-depthVector)
    );
}

Grid& GameGrid::GetLogical() {
    return logicalGrid;
}
