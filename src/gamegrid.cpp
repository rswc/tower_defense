#include "gamegrid.h"

#include "utility.h"
#include "grid.h"
#include "BaseMesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

GameGrid::GameGridPosition GameGrid::gridToModelPosition(Grid::GridPosition p) {
    if (!logicalGrid.isInsideGrid(p)) {
        return {-1, -1, -1};
    }
    
    return { p.row + halfRowScale, cellElevation(p), p.col + halfColScale };
}

inline float GameGrid::cellElevation(Grid::GridPosition p) {
    if (logicalGrid.isInsideGrid(p))
        return logicalGrid.isLand(p) ? .3f : 0;
    return .3f;
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

    // Lower-Left triangle
    appendTriangle(mesh.vertices, upperLeft, lowerLeft, lowerRight);
    appendTriangle(mesh.normals, 
        calculateTriangleNormal(upperLeft, lowerLeft, lowerRight),
        calculateTriangleNormal(lowerLeft, lowerRight, upperLeft),
        calculateTriangleNormal(lowerRight, upperLeft, lowerLeft));
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

    GameGridPosition innerDownRight = cellCenter + innerSquareFactor * dirVector[GAME_DIR_DOWNRIGHT];
    GameGridPosition innerUpRight = cellCenter + innerSquareFactor * dirVector[GAME_DIR_UPRIGHT];
    GameGridPosition innerDownLeft = cellCenter + innerSquareFactor * dirVector[GAME_DIR_DOWNLEFT];
    GameGridPosition innerUpLeft = cellCenter + innerSquareFactor * dirVector[GAME_DIR_UPLEFT];

    GameGridPosition outerDownRight = cellCenter + dirVector[GAME_DIR_DOWNRIGHT];
    GameGridPosition outerUpRight = cellCenter + dirVector[GAME_DIR_UPRIGHT];
    GameGridPosition outerDownLeft = cellCenter + dirVector[GAME_DIR_DOWNLEFT];
    GameGridPosition outerUpLeft = cellCenter + dirVector[GAME_DIR_UPLEFT];

    // Blessh this mesh
    
    // Inner square
    appendRectangle(
        mesh,
        innerUpRight,
        innerDownRight,
        innerUpLeft,
        innerDownLeft
    );

    // Upper edge strip
    appendRectangle(
        mesh,
        { outerUpRight.x, cellElevation({row - 1, column}), innerUpRight.z },
        innerUpRight,
        { outerUpLeft.x, cellElevation({row - 1, column}), innerUpLeft.z },
        innerUpLeft
    );

    // Right edge strip
    appendRectangle(
        mesh,
        { innerDownRight.x, cellElevation({row, column + 1}), outerDownRight.z },
        innerDownRight,
        { innerUpRight.x, cellElevation({row, column + 1}), outerUpRight.z },
        innerUpRight
    );

    // Lower edge strip
    appendRectangle(
        mesh,
        { outerDownLeft.x, cellElevation({row + 1, column}), innerDownLeft.z },
        innerDownLeft,
        { outerDownRight.x, cellElevation({row + 1, column}), innerDownRight.z },
        innerDownRight
    );

    // Left edge strip
    appendRectangle(
        mesh,
        { innerUpLeft.x, cellElevation({row, column - 1}), outerUpLeft.z },
        innerUpLeft,
        { innerDownLeft.x, cellElevation({row, column - 1}), outerDownLeft.z },
        innerDownLeft
    );

    // Upper-right corner
    appendRectangle(
        mesh,
        { innerUpRight.x, cellElevation({row, column + 1}), outerUpRight.z },
        innerUpRight,
        { outerUpRight.x, cornerElevation({row, column + 1}, {row - 1, column}, {row - 1, column + 1}), outerUpRight.z },
        { outerUpRight.x, cellElevation({row - 1, column}), innerUpRight.z }
    );

    // Lower-right corner
    appendRectangle(
        mesh,
        { innerDownRight.x, cellElevation({row, column + 1}), outerDownRight.z },
        { outerDownRight.x, cornerElevation({row, column + 1}, {row + 1, column}, {row + 1, column + 1}), outerDownRight.z },
        innerDownRight,
        { outerDownRight.x, cellElevation({row + 1, column}), innerDownRight.z }
    );

    // Upper-left corner
    appendRectangle(
        mesh,
        { outerUpLeft.x, cellElevation({row - 1, column}), innerUpLeft.z },
        innerUpLeft,
        { outerUpLeft.x, cornerElevation({row, column - 1}, {row - 1, column}, {row - 1, column - 1}), outerUpLeft.z },
        { innerUpLeft.x, cellElevation({row, column - 1}), outerUpLeft.z }
    );

    // Lower-left corner
    appendRectangle(
        mesh,
        { outerDownLeft.x, cellElevation({row + 1, column}), innerDownLeft.z },
        { outerDownLeft.x, cornerElevation({row, column - 1}, {row + 1, column}, {row + 1, column - 1}), outerDownLeft.z },
        innerDownLeft,
        { innerDownLeft.x, cellElevation({row, column - 1}), outerDownLeft.z }
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
        if (logicalGrid.isLand({r, c}))
        {
            makeFlatCell(mesh, r, c);
        } else {
            makeCanyonCell(mesh, r, c);
        }
    }
    return mesh;
}
    
BaseMesh GameGrid::generateBaseMesh(GameGrid::MeshVersion version) {
    GameGridMesh mesh;
    BaseMesh baseMesh;

    switch (version) {
        case MESH_V_FIRST: mesh = generateSimpleMesh(); break;
        case MESH_V_SECOND: mesh = generateWalledMesh(); break;
    }
    
    baseMesh.vertices = mesh.vertices;
    baseMesh.normals = mesh.normals;
    return baseMesh;
}