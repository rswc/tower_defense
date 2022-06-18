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
    
    GameGridPosition translateGridToGamePosition(Grid::GridPosition p, GameDirEnum dir); 
    void appendTriangle(std::vector<glm::vec4>& vertexArray, GameGridPosition a, GameGridPosition b, GameGridPosition c); 
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