#include "utility.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>


// Calculate Normal vector on Triangle A-B-C using vectors A->B and A->C
// Resulting Vector is a vector starting in A vertex 
// Assume: A, B,C are given in counter-clockwise order
glm::vec3 calculateTriangleNormal(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
    glm::vec3 crossProduct = glm::cross(B-A, C-A);
    return normalize(crossProduct);
}