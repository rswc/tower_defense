#pragma once 

#include <vector>

// Initialize empty matrix made of std::vector 
// Example usage: (initialize 5x4 matrix of one's)
// 1. std::vector<std::vector<int>> matrix; 
// 2. initializeMatrix(matrix, 5, 4, 1); 
template<typename T>
void initializeMatrix(std::vector<std::vector<T>>& matrix, int rows, int cols, T val) {
    assert (matrix.empty());
    for (int i = 0; i < rows; i++)
        matrix.emplace_back(cols, val);
}
