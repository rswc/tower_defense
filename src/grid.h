#pragma once

#include <vector>
#include <string>

enum {
    DIR_NONE    = -1,
    DIR_RIGHT   = 0,
    DIR_UP      = 1,
    DIR_LEFT    = 2,
    DIR_DOWN    = 3,
    DIR_N       = 4,
};

enum {
    GRID_ADJ_RIGHT  = 1 << DIR_RIGHT,
    GRID_ADJ_UP     = 1 << DIR_UP,
    GRID_ADJ_LEFT   = 1 << DIR_LEFT,
    GRID_ADJ_DOWN   = 1 << DIR_DOWN,
    GRID_OBSTACLE   = 1 << 4,
    GRID_START      = 1 << 5,
    GRID_EXIT       = 1 << 6,
    GRID_VISITED    = 1 << 7,
    GRID_PARENT     = 1 << 8,
};

enum {
    ERROR_NOERROR = 0,
    ERROR_BAD_SIZE,
};

int get_reverse_direction(int direction) {
    return (direction < DIR_N/2 ? direction + DIR_N/2 : direction - DIR_N/2);
}


class Grid {

    static constexpr int DIR_VEC_ROW[4] = {0, -1, 0, 1};
    static constexpr int DIR_VEC_COL[4] = {1, 0, -1, 0};

    static constexpr char MAP_OBSTACLE = 'X';
    static constexpr char MAP_EMPTY = '.';
    static constexpr char MAP_START = 'S';
    static constexpr char MAP_EXIT = 'E';
    
    struct position {
        int row, col;
        position(int r = 0, int c = 0) {}
    };

    position m_start, m_exit, m_size;
    std::vector<std::vector<int>> m_grid;

    bool is_map_valid(std::vector<std::string>& map); 
    bool is_position_valid(position p);
    int build_grid_from_map(std::vector<std::string>& map); 
    int build_grid_paths_bfs();
    
    Grid() {}

    // assume grid is ready
    void build_grid_paths() {
    }

    void init_grid(std::vector<std::string>& map) {
        if (!is_map_valid(map)) 
            return 1;
        m_map = map;
        build_grid();
    }

    bool is_valid(int row, int col) { return (unsigned) row < m_rows && (unsigned) col < m_cols; }

    void init(int height, int width) {
        if (m_rows > 0 || m_width > 0) reset();

        m_grid.assign(n);
        for (int i = 0; i < m_rows; i++) {
        }
    }

    void reset() { m_grid.clear(); m_rows = m_cols = 0; }

    int parse_file_map(const std::vector<std::string>& map) {
        // Assume cols and rows initialized

        for (int i = 0; i < m_rows; i++) {
        }
    }

    int load_map(std::) {
        std::vector<string> map;
        for (string &row : map) infile >> row;

        int rows = 0, cols = 0;
        rows = map.size();
        if (rows > 0) cols = map[0].size();

        if (rows == 0 || cols == 0) {
            return ERROR_BAD_SIZE;
        }

        init(rows, cols);
        parse_map(map);

        return ERROR_NOERROR;
    }
    }

public:
    Grid(int height, int width, float scale = 1.f) { 
        assert(height > 0 && width > 0 && "Grid constructor called with non-positive arguments!");
        init(height, width);
    }

    getNextDirection(Point p) 
    std:: getQuadraticBezier()

};

void test_map() {
}