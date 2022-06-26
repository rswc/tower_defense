#pragma once

#include "grid.h"
#include "BaseMesh.h"

#include <vector>
#include <glm/glm.hpp>

class GameGrid {
public:
    struct GameGridMesh {
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec4> normals;
        std::vector<glm::vec3> textures;

        operator BaseMesh() {
            BaseMesh baseMesh;
            baseMesh.vertices = vertices;
            baseMesh.normals = normals;
            return baseMesh;
        }
    };


    struct GamePath {
        float start_time = 0.5f;
        float end_time = 0.5f;
        std::vector<glm::vec4> points;
    };

private:
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

    const float halfRowScale = 0.5f;
    const float halfColScale = 0.5f;

    const float innerSquareFactor = 0.6f;
    const float elevationStep = .3f;

    const float borderExtrusionParallel = .8f;
    const float borderExtrusionPerpendicular = 1.4f;

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

    const GameGridPosition depthVector = { 0.f, -elevationStep, 0.f};
    
    GameGridPosition gridToModelPosition(Grid::GridPosition p);
    inline float cellElevation(Grid::GridPosition p);
    float cornerElevation(Grid::GridPosition a, Grid::GridPosition b, Grid::GridPosition c);

    void appendTriangle(std::vector<glm::vec4>& vertexArray, GameGridPosition a, GameGridPosition b, GameGridPosition c);
    void appendTriangle(std::vector<glm::vec3>& vertexArray, GameGridPosition a, GameGridPosition b, GameGridPosition c);
    void appendRectangle(
        GameGrid::GameGridMesh& mesh,
        GameGridPosition upperRight,
        GameGridPosition lowerRight,
        GameGridPosition upperLeft,
        GameGridPosition lowerLeft
    );
    
    void makeFlatCell(GameGridMesh& mesh, int row, int column);
    void makeCanyonCell(GameGridMesh& mesh, int row, int column);
    void makeGridBorder(GameGridMesh& mesh, int rows, int columns);
    
    GameGridMesh generateSimpleMesh();
    GameGridMesh generateWalledMesh();
    
    GamePath generateGamePath(const Grid& grid);
    
public:
    enum : MeshVersion {
        MESH_V_FIRST = 1,
        MESH_V_SECOND = 2,
        MESH_V_THIRD = 3,
    };

    GamePath gamePath;
    GameGridMesh generateBaseMesh(MeshVersion version);
    GameGrid(const std::vector<std::string>& map) 
        : logicalGrid(map), gamePath(generateGamePath(logicalGrid)) {}
};