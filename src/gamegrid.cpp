#include "gamegrid.h"

#include "utility.h"
#include "grid.h"
#include "BaseMesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

GameGrid::GameGridPosition GameGrid::translateGridToGamePosition(Grid::GridPosition p, GameGrid::GameDirEnum dir = GAME_DIR_NONE) {
    if (!logicalGrid.isInsideGrid(p)) {
        return {-1, -1, -1};
    }

    float hScale = (logicalGrid.isLand(p) ? 10.f : 0);
    
    const float halfRowScale = 0.5f;
    const float halfColScale = 0.5f;
    
    GameGridPosition cellCenter { p.row + halfRowScale, hScale, p.col + halfColScale };
    const GameGridPosition dirVector[GAME_DIR_N] = {
        { 0.f, 0.f, halfColScale }, 
        { -halfRowScale, 0.f, halfColScale }, 
        { -halfRowScale, 0.f, 0.f }, 
        { -halfRowScale, 0.f, -halfColScale }, 
        { 0.f, 0.f, -halfColScale }, 
        { halfRowScale, 0.f, -halfColScale }, 
        { halfRowScale, 0.f, 0.f }, 
        { halfRowScale, 0.f, halfColScale }, 
    };

    if (dir == GAME_DIR_NONE) {
        return cellCenter;
    }
    else {
        return cellCenter + dirVector[dir];
    }
}
    
void GameGrid::appendTriangle(std::vector<glm::vec4>& vertexArray, GameGrid::GameGridPosition a, 
    GameGrid::GameGridPosition b, GameGrid::GameGridPosition c) {
    vertexArray.emplace_back(a, 1.f);
    vertexArray.emplace_back(b, 1.f);
    vertexArray.emplace_back(c, 1.f);
}
    
GameGrid::GameGridMesh GameGrid::generateSimpleMesh() {
    GameGridMesh mesh;
    
    for (int r = 0; r < logicalGrid.m_rows; r++) 
    for (int c = 0; c < logicalGrid.m_cols; c++) {
        GameGridPosition downRight = translateGridToGamePosition({r, c}, GAME_DIR_DOWNRIGHT);
        GameGridPosition upRight = translateGridToGamePosition({r, c}, GAME_DIR_UPRIGHT);
        GameGridPosition downLeft = translateGridToGamePosition({r, c}, GAME_DIR_DOWNLEFT);
        GameGridPosition upLeft = translateGridToGamePosition({r, c}, GAME_DIR_UPLEFT);
        
        // Upper-Right triangle 
        appendTriangle(mesh.vertices, upLeft, downRight, upRight);
        appendTriangle(mesh.normals, 
            calculateTriangleNormal(upLeft, downRight, upRight),
            calculateTriangleNormal(downRight, upRight, upLeft),
            calculateTriangleNormal(upRight, upLeft, downRight));

        // Lower-Left triangle
        appendTriangle(mesh.vertices, upLeft, downLeft, downRight);
        appendTriangle(mesh.normals, 
            calculateTriangleNormal(upLeft, downLeft, downRight),
            calculateTriangleNormal(downLeft, downRight, upLeft),
            calculateTriangleNormal(downRight, upLeft, downLeft));
    }
    return mesh;
}

    
BaseMesh GameGrid::generateBaseMesh(GameGrid::MeshVersion version) {
    GameGridMesh mesh;
    BaseMesh baseMesh;

    switch (version) {
        case MESH_V_FIRST: mesh = generateSimpleMesh();
    }
    
    baseMesh.vertices = mesh.vertices;
    baseMesh.normals = mesh.normals;
    return baseMesh;
}