#include "towermanager.h"
#include <memory>

int TowerManager::countFreeTowers() {
    int cnt = 0;
    for (auto tower_ptr : towers) { 
        if (!tower_ptr->active) cnt++;
    }
    return cnt;
}

void TowerManager::reactivateTower(glm::vec3 pos) {
    for (auto tower : towers) { 
        if (tower->active) continue;
        tower->restart();
        tower->SetPosition(pos);
        break;
    }
}

void TowerManager::deactivateTower(glm::vec3 pos) {
    for (auto tower : towers) { 
        if (!tower->active) continue;
        
        auto diff = tower->GetPosition() - pos;
        if (fabs(diff.x) < Tower::EPS && fabs(diff.z) < Tower::EPS) {
            tower->deactivate();
            return;
        }
    }
}

std::unique_ptr<Tower> TowerManager::createTower(BulletManager * bulletman, MobManager * mobman, glm::vec3 position) {
    std::unique_ptr<Tower> new_tower = std::make_unique<Tower>(bulletman, mobman);
    towers.push_back(new_tower.get());
    new_tower->restart();
    new_tower->SetPosition(position);
    return new_tower;
}