#pragma once

#include "grid.h"

struct GameGridMeshHandler {
    int vertexCount;
    float *pos, *normals;
};

template <int scaleX, int scaleY, int scaleZ>
class GameGrid {
    using MeshVersion = int;
    using GameGridPosition = glm::vec3;

    enum : MeshVersion {
        MESH_V_FIRST = 1,
        MESH_V_SECOND = 2,
        MESH_V_THIRD = 3,
    };
           
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

    Grid logicalGrid;
    
    struct GameGridMesh {
        int vertexCount = 0;
        std::vector<float> positions;
        std::vector<float> normals;
    };
    
    
    GameGridPosition translateGridToGamePosition(Grid::GridPosition p, GameDirEnum dir = GAME_DIR_NONE) {
        if (!logicalGrid.isInsideGrid(p)) {
            return {-1, -1, -1};
        }

        const float rowScale = scaleX;
        const float colScale = scaleZ;
        float hScale = (logicalGrid.isLand() ? scaleY : 0);
        
        const float halfRowScale = rowScale / 2.f;
        const float halfColScale = colScale / 2.f;
        
        GameGridPosition cellCenter { rowScale * p.row + halfRowScale, colScale * p.col + halfColScale, hScale };
        const Grid::GridPosition dirVector[GAME_DIR_N] = {
            { 0.f, halfColScale, 0.f }, 
            { -halfRowScale, halfColScale, 0.f }, 
            { -halfRowScale, 0.f, 0.f }, 
            { -halfRowScale, -halfColScale, 0.f }, 
            { 0.f, -halfColScale, 0.f }, 
            { halfRowScale, -halfColScale, 0.f }, 
            { halfRowScale, 0.f, 0.f }, 
            { halfRowScale, halfColScale, 0.f }, 
        };
    
        if (dir == GAME_DIR_NONE) 
        else {
            return cellCenter + dirVector[dir];
        }
    }
    
    void appendTriangle(std::vector<float>& vertexArray, GameGridPosition a, GameGridPosition b, GameGridPosition c) {
        vertexArray.push_back(a.x, a.y, a.z);
        vertexArray.push_back(b.x, b.y, b.z);
        vertexArray.push_back(c.x, c.y, c.z);
    }
    
    GameGridMesh generateSimpleMesh() {
        GameGridMesh mesh;
        
        for (int r = 0; r < logicalGrid.m_rows; r++) 
        for (int c = 0; c < logicalGrid.m_cols; c++) {
            GameGridPosition downRight = translateGridToGamePosition({r, c}, GAME_DIR_DOWNRIGHT);
            GameGridPosition upRight = translateGridToGamePosition({r, c}, GAME_DIR_UPRIGHT);
            GameGridPosition downLeft = translateGridToGamePosition({r, c}, GAME_DIR_DOWNLEFT);
            GameGridPosition upLeft = translateGridToGamePosition({r, c}, GAME_DIR_UPLEFT);
            
            // Upper-Right triangle 
            appendTriangle(mesh.positions, upLeft, downRight, upRight);
            appendTriangle(mesh.normals, 
                calculateTriangleNormal(upLeft, downRight, upRight));
                calculateTriangleNormal(downRight, upRight, upLeft),
                calculateTriangleNormal(upRight, upLeft, downRight),

            // Lower-Left triangle
            appendTriangle(mesh.positions, upLeft, downLeft, downRight);
            appendTriangle(mesh.normals, 
                calculateTriangleNormal(upLeft, downLeft, downRight),
                calculateTriangleNormal(downLeft, downRight, upLeft),
                calculateTriangleNormal(downRight, upLeft, downLeft));
        }
        mesh.vertexCount = mesh.positions.size() / 3;
        return mesh;
    }
    
    GameGridMesh generateMesh(MeshVersion version) {
        GameGridMesh mesh;
        
        switch (version) {
            case MESH_V_FIRST: mesh = generateSimpleMesh();
        }
        return mesh;
    }
    
    GameGrid(std::vector<std::string>& map, MeshVersion meshVersion = MESH_V_FIRST) 
        : logicalGrid(map),  {
    }
};