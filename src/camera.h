#pragma once

#include <glm/matrix.hpp>
#include "transform.h"
#include "utility.h"


class Camera : public Transformable
{
public:
    struct PointLight {
        glm::vec3 position;
        
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float A;
        float B;
        float C;

        PointLight(
            glm::vec3 position,
            glm::vec3 ambient,
            glm::vec3 diffuse,
            glm::vec3 specular,
            float A,
            float B,
            float C
        ) : position(position),
            ambient(ambient),
            diffuse(diffuse),
            specular(specular),
            A(A), B(B), C(C) {};
    };

protected:
    constexpr static glm::vec4 FORWARD = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    constexpr static glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 p, inverseP;
    glm::vec2 screenSize;
    std::vector<PointLight> lights;

    void SetP(glm::mat4 matrix);
public:
    static constexpr int numPointLights = 1;

    Camera();

    virtual glm::mat4 GetV() const;
    virtual glm::mat4 GetP() const;
    glm::vec3 GetForward() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetRight() const;

    void setLookAt(float camX, float camZ);
    virtual void MoveCamera(
            float frontSpeedMove,
            float rightSpeedMove, 
            float pitch, 
            float speed, 
            float roll, 
            float deltaTime
            ) {};

    virtual void SetCameraRotationBlock(bool block) {};
    virtual void SetCameraHeightCap(bool toggle, float cap) {};
    virtual void ZoomCamera(float fov) {};

    void SetScreenSize(float width, float height);
    glm::vec2 GetScreenSize() const;
    glm::mat4 GetOrtho() const;

    void PushLight(PointLight light);
    std::vector<PointLight> GetLights() const;

    Ray ViewportToRay(float x, float y) const;

};

