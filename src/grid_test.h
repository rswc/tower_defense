#pragma once

#include <iostream>

#include "grid.h"


class GridTest {
    const std::string rowBreak = std::string(60, '-');
    
    char decode_direction(Grid::DirEnum dir) {
        const char dir_symbol[4] = { '>', '^', '<', 'v' };
        if (dir == Grid::DIRECTION_NONE) return 'x';
        else if (dir > Grid::DIRECTION_NONE && dir < Grid::DIRECTION_N) {
            return dir_symbol[dir];
        }
        else {
            return '?';
        }
    }

    bool testFindingPath(const Grid::GridMap& map, const Grid::GridPath& path) {
        std::cerr << "Running GridPath Test\n";
        for (const auto &row : map) {
            std::cerr << row << "\n";
        }

        Grid grid = Grid(map);
        Grid::GridPath grid_path = grid.m_path;

        if (grid_path.size() != path.size()) {
            std::cerr << "GridPath size mismatch!\n";
            return false;
        }
        
        bool path_valid = true;
        for (int i = 0; i < (int) path.size(); i++) {
            if (path[i] != grid_path[i]) {
                std::cerr << "GridPath " << i << "-th position mismatch!\n";
                path_valid = false;
            }
            std::cerr << "[" << i << "]: {" << path[i].first.row << ", " 
                << path[i].first.col << "}, next: "<< decode_direction(path[i].second) << std::endl;
        }
        return path_valid;
    }
    
    bool batchTestFindingPath() {
        bool all_valid = true;
        {
            Grid::GridMap map {{
                "xxx", "xSx", "x.x", "xEx", "xxx", 
            }};
            
            Grid::GridPath path {{
                {{1, 1}, Grid::DIRECTION_DOWN}, 
                {{2, 1}, Grid::DIRECTION_DOWN}, 
                {{3, 1}, Grid::DIRECTION_NONE},
            }};

            std::cerr << rowBreak << std::endl;
            if (!testFindingPath(map, path)) {
                all_valid = false;
            } else {
                std::cerr << "[OK]" << std::endl;
            }
            std::cerr << rowBreak << std::endl;
        }
        { 
            Grid::GridMap map {{
                "xxxxxx",
                "xS...x",
                "xxxx.x",
                "x....x",
                "x.xxxx",
                "x.x.Ex",
                "x.x.xx",
                "x...xx",
                "xxxxxx"
            }};

            Grid::GridPath path {{
                {{1, 1}, Grid::DIRECTION_RIGHT}, 
                {{1, 2}, Grid::DIRECTION_RIGHT}, 
                {{1, 3}, Grid::DIRECTION_RIGHT},
                {{1, 4}, Grid::DIRECTION_DOWN},
                {{2, 4}, Grid::DIRECTION_DOWN},
                {{3, 4}, Grid::DIRECTION_LEFT},
                {{3, 3}, Grid::DIRECTION_LEFT},
                {{3, 2}, Grid::DIRECTION_LEFT},
                {{3, 1}, Grid::DIRECTION_DOWN},
                {{4, 1}, Grid::DIRECTION_DOWN},
                {{5, 1}, Grid::DIRECTION_DOWN},
                {{6, 1}, Grid::DIRECTION_DOWN},
                {{7, 1}, Grid::DIRECTION_RIGHT},
                {{7, 2}, Grid::DIRECTION_RIGHT},
                {{7, 3}, Grid::DIRECTION_UP},
                {{6, 3}, Grid::DIRECTION_UP},
                {{5, 3}, Grid::DIRECTION_RIGHT},
                {{5, 4}, Grid::DIRECTION_NONE},
            }};

            std::cerr << rowBreak << std::endl;
            if (!testFindingPath(map, path)) {
                all_valid = false;
            } else {
                std::cerr << "[OK]" << std::endl;
            }
            std::cerr << rowBreak << std::endl;
        }
        return all_valid;
    }
    
    
public:
    GridTest() {
        assert(batchTestFindingPath());
    }
};

