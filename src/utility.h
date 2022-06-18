#ifndef UTILITY_H_
#define UTILITY_H_

#include <glm/glm.hpp>

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

// Calculate Normal vector on Triangle A-B-C using vectors A->B and A->C
// Resulting Vector is a vector starting in A vertex 
// Assume: A, B,C are given in counter-clockwise order
glm::vec3 calculateTriangleNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C);

#endif