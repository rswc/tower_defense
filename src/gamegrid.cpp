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
    
GameGrid::GameGridMesh GameGrid::generateSimpleMesh() {
    GameGridMesh mesh;
    
    for (int r = 0; r < logicalGrid.m_rows; r++) 
    for (int c = 0; c < logicalGrid.m_cols; c++) {
        makeFlatCell(mesh, r, c);
    }
    return mesh;
}
    
BaseMesh GameGrid::generateBaseMesh(GameGrid::MeshVersion version) {
    GameGridMesh mesh;
    BaseMesh baseMesh;

    switch (version) {
        case MESH_V_FIRST: mesh = generateSimpleMesh(); break;
    }
    
    baseMesh.vertices = mesh.vertices;
    baseMesh.normals = mesh.normals;
    return baseMesh;
}