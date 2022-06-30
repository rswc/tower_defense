#pragma once

#include "gameobject.h"
#include "mobmanager.h"
#include "BaseMesh.h"

class BulletManager;

class Tower : public GameObject {
    friend class TowerManager;
private:
    
    static constexpr float EPS = 1e-9;
    float refreshTime = 5.0f;
    float shootTime = 1.0f;
    float radius = 10.0f;
    float towerHeight = 1.0f;

    float timeLeft = 0;
    bool active = false;
    bool ready = false;

    MobManager * mobman = nullptr;
    BulletManager * bulletman = nullptr;
    BaseMesh *mesh = nullptr;
    GLuint tex;

public:
    Tower(BulletManager * bman, MobManager * mman);
    
    void restart() {
        timeLeft = refreshTime;
        ready = false;
        active = true;
    }

    inline void deactivate() { active = false; } 
    
    glm::vec3 getShootPoint() const {
        return GetPosition() + glm::vec3(0.0f, towerHeight, 0.0f);
    }
    
    inline bool inRange(glm::vec3 position) {
        glm::vec3 diff = position - getShootPoint();
        return diff.length() < radius;
    }
    void shoot();
    
    void Draw(const Camera& camera) const override;

    void Update(double deltaTime)  override {
        if (!active) return;

        if (!ready && timeLeft >= 0) { timeLeft -= deltaTime; }
        else { ready = true; shoot(); }
    }
};
