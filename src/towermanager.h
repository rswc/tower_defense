#pragma once

#include "tower.h"

class TowerManager {
    std::vector<Tower*> towers;

public: 
    TowerManager() = default;
    ~TowerManager() = default;

    int countFreeTowers();
    void reactivateTower(glm::vec3 position); 
    void deactivateTower(glm::vec3 position);
    std::unique_ptr<Tower> createTower(BulletManager * bulletman, MobManager * mobman, glm::vec3 position);
};
