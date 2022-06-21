#pragma once

#include "grid.h"
#include "BaseMesh.h"

#include <vector>
#include <glm/glm.hpp>

class GameGrid {
    Grid logicalGrid;

    using MeshVersion = int;
    using GameGridPosition = glm::vec3;
           
    using GameDirEnum = int;
    
    enum : GameDirEnum {
        GAME_DIR_NONE = -1,
        GAME_DIR_RIGHT,
        GAME_DIR_UPRIGHT,
        GAME_DIR_UP,
        GAME_DIR_UPLEFT,
        GAME_DIR_LEFT,
        GAME_DIR_DOWNLEFT,
        GAME_DIR_DOWN,
        GAME_DIR_DOWNRIGHT,
        GAME_DIR_N
    };

    struct GameGridMesh {
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec4> normals;
    };

    const float halfRowScale = 0.5f;
    const float halfColScale = 0.5f;

    const float innerSquareFactor = 0.6f;

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
    
    GameGridPosition gridToModelPosition(Grid::GridPosition p);
    inline float cellElevation(Grid::GridPosition p);
    void appendTriangle(std::vector<glm::vec4>& vertexArray, GameGridPosition a, GameGridPosition b, GameGridPosition c);
    void appendRectangle(
        GameGridMesh& mesh,
        GameGridPosition upperRight,
        GameGridPosition lowerRight,
        GameGridPosition upperLeft,
        GameGridPosition lowerLeft
    );
    void makeFlatCell(GameGridMesh& mesh, int row, int column);
    GameGridMesh generateSimpleMesh();

    
public:
    enum : MeshVersion {
        MESH_V_FIRST = 1,
        MESH_V_SECOND = 2,
        MESH_V_THIRD = 3,
    };
    BaseMesh generateBaseMesh(MeshVersion version);
    GameGrid(const std::vector<std::string>& map) : logicalGrid(map) {}
};