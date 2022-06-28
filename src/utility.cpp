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

// Calculate Point on Quadratic Bezier Curve using linear interpolation - glm::mix
// time argument is in range [0, 1]
// Assume: A, B, C are given in correct order -> Bezier Curve between A - C with B as point inbetween
glm::vec3 quadraticBezierCurve(glm::vec3 A, glm::vec3 B, glm::vec3 C, float time) {
    // glm::vec3 interAB = A * (1.0f - time) + B * time;
    // glm::vec3 interBC = B * (1.0f - time) + C * time;
    // return interAB * (1.0f - time) + interBC * time;

    glm::vec3 interAB = glm::mix(A, B, time); 
    glm::vec3 interBC = glm::mix(B, C, time); 
    return glm::mix(interAB, interBC, time);
}