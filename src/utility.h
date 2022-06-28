#ifndef UTILITY_H_
#define UTILITY_H_

#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>

#include <vector>

struct Plane
{
    glm::vec3 point;
    glm::vec3 normal;

    Plane(glm::vec3 point, glm::vec3 normal) : point(point), normal(normal) {}

    Plane ToWorldSpace(glm::mat4 model) const {
        return Plane(
            model * glm::vec4(point, 1.f),
            model * glm::vec4(normal, 0.f)
        );
    }
};

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(glm::vec3 origin, glm::vec3 direction) : origin(origin), direction(direction) {}

    Ray ToWorldSpace(glm::mat4 model) const {
        return Ray(
            model * glm::vec4(origin, 1.f),
            model * glm::vec4(direction, 0.f)
        );
    }

    bool Intersect(Plane plane, glm::vec3& hit) const {
        float t;

        bool didHit = glm::intersectRayPlane(
            origin,
            direction,
            plane.point,
            plane.normal,
            t
        );

        hit = origin + t * direction;

        return didHit;
    }
};

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