#pragma once

#include "gameobject.h"

class BaseMesh;
typedef unsigned int GLuint;

class BulletObject : public GameObject {
    friend class BulletManager;

private:

    static constexpr float EPS = 1e-9;
    float towerMobRatio = 0.5;
    float midPointHeight = 2.f; 

    float flyTime = 0;
    float inAirTime = 0; 
    bool active = false;
    BaseMesh *mesh = nullptr;
    glm::vec3 src, dst, mid, curr;
    GLuint tex;

public:
    
    void restart(glm::vec3 A, glm::vec3 B, float flyingTime) {
        active = true;
        flyTime = flyingTime;
        inAirTime = 0;
        src = A;
        dst = B;
        SetPosition(A);
        mid = glm::mix(src, dst, towerMobRatio);
        mid.y = midPointHeight;
    }
    
    BulletObject();

    void Draw(const Camera& camera) const override;

    void Update(double deltaTime)  override {
        if (!active) return;
        
        if (inAirTime + deltaTime >= flyTime) {
            active = false;
        } else {
            inAirTime += deltaTime;
            SetPosition(quadraticBezierCurve(src, mid, dst, inAirTime / flyTime));
        }
    }
};
