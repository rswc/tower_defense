#pragma once

#include "gameobject.h"
#include "mobmanager.h"
#include "BaseMesh.h"

class BulletManager;

class Tower : public GameObject {
    friend class TowerManager;
private:
    
    static constexpr float EPS = 1e-9;
    static constexpr float refreshTime = 5.0f;
    static constexpr float shootTime = 1.0f;
    static constexpr float radius = 10.0f;
    static constexpr float towerHeight = 1.0f;

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
    
    void shoot() {
        if (!ready) return;

        for (MobObject * mob : mobman->mobs) {
            if (!mob->isActive()) continue;

            std::cout << "Checking mob with id: " << mob->id() << std::endl;
            if (!mob->shootable(shootTime)) { 
                std::cout << "Mob is not shootable..." << std::endl; 
                continue;
            }
            if (!inRange(mob->getModelHitCoordinates(shootTime))) { 
                std::cout << "Mob not in range.." << std::endl;
                continue;
            }
            std::cout << "Shooting that naggers " << mob->id() << std::endl;
            mob->markDead(shootTime);
            timeLeft = refreshTime;
            ready = false;
            return;
        }
    }

    void Draw(const Camera& camera) const override;

    void Update(double deltaTime)  override {
        if (!active) return;

        if (!ready && timeLeft >= 0) { timeLeft -= deltaTime; }
        else { ready = true; shoot(); }
    }
};
