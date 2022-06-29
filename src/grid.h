#pragma once

#include <cassert>
#include <vector>
#include <string>

// Grid Class is responsible for converting input game map into logical grid
// Main functionalities are: 
class GameGrid;
//  - creating a GridPath for mobs to traverse
class GridTest;
//  - generating GridMesh for in-game visualization 
class Grid {
    // GameGrid Class makes effective use of Grid class in Game World
    friend GameGrid; 
    // GridTest Class is used for effective testing Grid Class features 
    friend GridTest;

public:
    // GridPosition is main representation of coordinates inside the grid
    // - constexpr class: can be defined constant 
    struct GridPosition {
        int row, col;
        constexpr GridPosition(int r = 0, int c = 0) : row(r), col(c) {}
        constexpr GridPosition operator+(GridPosition p) { return { row + p.row, col + p.col }; }
        constexpr GridPosition operator-(GridPosition p) { return { row + p.row, col + p.col }; }
        constexpr bool operator==(const GridPosition& p) const { return row == p.row && col == p.col; }
    };

private:    
    // DirEnum is used for Grid representation of possible directions
    using DirEnum = int;
    // Represents binary flag indicating whether move in specified direction is possible
    using DirFlag = int;
    // Bitset of DirFlags
    using DirBitset = int;
    // Represents possible directions for every cell on the grid
    using DirMatrix = std::vector<std::vector<DirBitset>>;

    enum : DirEnum {
        DIRECTION_NONE = -1,
        DIRECTION_RIGHT, 
        DIRECTION_UP,
        DIRECTION_LEFT,
        DIRECTION_DOWN,
        DIRECTION_N, 
    };

    enum : DirFlag {
        AVAILABLE_RIGHT = 1 << DIRECTION_RIGHT,
        AVAILABLE_UP    = 1 << DIRECTION_UP,
        AVAILABLE_LEFT  = 1 << DIRECTION_LEFT,
        AVAILABLE_DOWN  = 1 << DIRECTION_DOWN,
    };

    // Return DirectionEnum in opposite direction 
    // Assume: dir is one of: DIRECTION_RIGHT, DIRECTION_UP, DIRECTION_LEFT, DIRECTION_DOWN,
    DirEnum oppositeDirection(DirEnum dir) const {
        // Because of counter-clockwise ordering, opposite direction is (dir + 2) mod 4
        return (dir < DIRECTION_N/2 ? dir + DIRECTION_N/2 : dir - DIRECTION_N/2);
    }

    // GridMap is input representation of Game Map
    using GridMap = std::vector<std::string>;
    using GridMapSymbol = char;
    
    enum : GridMapSymbol {
        MAP_START = 'S', 
        MAP_PATH = '.',
        MAP_LAND = 'x',
        MAP_EXIT = 'E',
        MAP_TOWER = 'T',
        MAP_TREE = '@',
    };

    // Return direction advanced by one cell in specified direction
    GridPosition moveInDirection(GridPosition p, DirEnum dir) const {
        const GridPosition offset[4] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
        return p + offset[dir];
    }
    
    bool isLand(GridPosition p) const {
        return m_map[p.row][p.col] == MAP_LAND;
    }
    
    bool isTower(GridPosition p) const {
        return m_map[p.row][p.col] == MAP_TOWER;
    }

    // GridPath Class stores path created by the grid class
    // Assumption: GridPath does not contain branches
    using GridPathPosition = std::pair<GridPosition, DirEnum>;
    using GridPath = std::vector<GridPathPosition>;

    GridMap m_map;
    int m_rows, m_cols;
    DirMatrix m_available_dirs;
    GridPath m_path;
    std::vector<GridPosition> m_trees;
    
    DirMatrix translateGridMapToDirMatrix(const GridMap &map);
    GridPath findGridPath();
    std::vector<GridPosition> findTilesOfType(GridMapSymbol symbol) const;

public:
    Grid(const std::vector<std::string>& map) { 
        m_map = map;
        m_rows = map.size();
        m_cols = map[0].size();
        m_available_dirs = translateGridMapToDirMatrix(map);

        m_path = findGridPath();
        m_trees = findTilesOfType(MAP_TREE);
    }

    int GetRows() const;
    int GetCols() const;
    
    bool isInsideGrid(GridPosition p) const {
        // dirty hack: casting to unsigned allows for faster implicit check for p.row > 0 and p.col > 0 
        return (unsigned) p.row < (unsigned) m_rows && (unsigned) p.col < (unsigned) m_cols;
    }

    bool TryPlaceTower(GridPosition p);
    std::vector<GridPosition> GetTreePositions() const;
};

