#include "grid.h"

#include "utility.h"

Grid::DirMatrix Grid::translateGridMapToDirMatrix(const Grid::GridMap &map) {
    assert(map.size() > 0 && map[0].size() > 0);
    
    GridPosition size { (int) map.size(), (int) map[0].size() };

    DirMatrix available_dirs;
    initializeMatrix(available_dirs, size.row, size.col, DirBitset(0));

    for (int r = 0; r < size.row; r++) 
    for (int c = 0; c < size.col; c++) {
        if (map[r][c] == MAP_LAND) continue;

        GridPosition current {r, c};

        for (DirEnum dir = DIRECTION_NONE + 1; dir < DIRECTION_N; dir++) {
            GridPosition adjacent = moveInDirection(current, dir);

            if ((unsigned) adjacent.row < (unsigned) size.row && (unsigned) adjacent.col < (unsigned) size.col 
                && map[adjacent.row][adjacent.col] != MAP_LAND) {
                available_dirs[r][c] |= (1 << dir);
            }
        }
    }

    return available_dirs;
}

Grid::GridPath Grid::findGridPath() {
    GridPosition startPosition { -1, -1 };
    GridPosition exitPosition { -1, -1 };
    
    for (int r = 0; r < m_rows; r++) 
    for (int c = 0; c < m_cols; c++) {
        if (m_map[r][c] == MAP_START) 
            startPosition = { r, c };
        else if (m_map[r][c] == MAP_EXIT) 
            exitPosition = { r, c };
    }
    
    GridPath gridPath;
    GridPosition current = startPosition;
    DirEnum nextDir = DIRECTION_NONE, lastDir = DIRECTION_NONE;

    do {
        lastDir = nextDir;
        nextDir = DIRECTION_NONE;

        for (DirEnum dir = DIRECTION_NONE + 1; dir < DIRECTION_N; dir++) {
            if (dir == oppositeDirection(lastDir)) continue;

            if (m_available_dirs[current.row][current.col] & (1 << dir)) {
                nextDir = dir;
            }
        }
        
        gridPath.emplace_back(current, nextDir);

        if (nextDir != DIRECTION_NONE) {
            current = moveInDirection(current, nextDir);
        }
    } while (nextDir != DIRECTION_NONE);
    
    assert(current == exitPosition);
    return gridPath;
}