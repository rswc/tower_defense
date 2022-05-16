#include "grid.h"
#include <cassert>
#include <queue>

bool Grid::is_position_valid(Grid::position pos) {
    return (unsigned) pos.row < m_size.row && (unsigned) pos.col < m_size.col;
}

int Grid::build_grid_paths_bfs() {
    std::queue<position> q;
    q.push(m_start);
    m_grid[m_start.row][m_start.col] |= GRID_VISITED;

    while (q.size()) {
        position u = q.front(); q.pop();

        for (int d = DIR_NONE + 1; d < DIR_N; d++) {
            position v { u.row + DIR_VEC_ROW[d], u.col + DIR_VEC_COL[d] };

            if (is_position_valid(v) && (m_grid[v.row][v.col] & (GRID_OBSTACLE | GRID_VISITED) == 0)) {
                // disallow cycle edges
                m_grid[v.row][v.col] &= ~(1 << get_reverse_direction(d));
                q.push(v);
                // how to erase paths from graph? 
                GRID_VISITED
            }
        }
    }
}

/*
* Initialize structure, build grid matrix from map, start dfs
*/
int Grid::build_grid_from_map(std::vector<std::string>& map) {
    m_size = position(map.size(), map[0].size());

    assert(m_grid.empty());
    for (int row = 0; row < m_size.row; row++) 
        m_grid.emplace_back(m_size.col, 0);
    
    for (int r = 0; r < m_size.row; r++) {
    for (int c = 0; c < m_size.col; c++) {
        switch (map[r][c]) {
            case MAP_OBSTACLE:  {
                m_grid[r][c] |= GRID_OBSTACLE;
                continue;
            } break;
            case MAP_START: {
                m_grid[r][c] |= GRID_START;
                m_start = position(r, c);
            } break;
            case MAP_EXIT: {
                m_grid[r][c] |= GRID_EXIT;
                m_exit = position(r, c);
            } break;
            case MAP_EMPTY: break;
            default: return 1;
        }

        for (int d = DIR_NONE + 1; d < DIR_N; d++) {
            int r2 = r + DIR_VEC_ROW[d];
            int c2 = r + DIR_VEC_COL[d];
            
            if (is_position_valid({r2, c2}) && map[r2][c2] != MAP_OBSTACLE) {
                m_grid[r][c] |= (1 << d); // mark direction as adjacent
            }
        }
    }}
}

/*
* Check constraints on string map like number of starting points, obstacles 
* and unknown characters present
*/
bool Grid::is_map_valid(std::vector<std::string>& map) {
    if (map.size() == 0 || map[0].size() == 0) 
        return false;
    
    int rows = map.size();
    int cols = map[0].size();

    // obstacle on edges
    for (int r = 0; r < rows; r++) {
        if (map[r][0] != MAP_OBSTACLE || map[r][cols - 1] != MAP_OBSTACLE) 
            return false;
    }

    for (int c = 0; c < cols; c++) {
        if (map[0][c] != MAP_OBSTACLE || map[rows - 1][c] != MAP_OBSTACLE)
            return false;
    }

    int start_count = 0;
    int exit_count = 0;
    for (auto &row : map) {
        for (auto &symbol : row) {
            switch (symbol) {
                case MAP_START: start_count++; break;
                case MAP_EXIT: exit_count++; break;
                case MAP_OBSTACLE: 
                case MAP_EMPTY: break;
                default: return false;
            }
        }
    }

    return (start_count == 1 && exit_count == 1);
}