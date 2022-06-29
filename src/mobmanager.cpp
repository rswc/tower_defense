#include "mobmanager.h"

#include <memory>

int MobManager::countFreeMobs() {
    int cnt = 0;
    for (auto mob_ptr : mobs) { 
        if (!mob_ptr->active) cnt++;
    }
    return cnt;
}

void MobManager::reactivateMob() {
    for (auto mob_ptr : mobs) { 
        if (mob_ptr->active) continue;

        std::cerr << "Reactivating mob with id: " << mob_ptr->m_id << ", assign new id: " << nextMobId << std::endl;

        mob_ptr->m_id = nextMobId++;
        mob_ptr->restart();
        break;
    }
}

std::unique_ptr<MobObject> MobManager::createMob(GameGrid* gameGridPtr) {
    std::cerr << "Creating mob with new id: " << nextMobId << std::endl;
    std::unique_ptr<MobObject> new_mob = std::make_unique<MobObject>(gameGridPtr, nextMobId++);
    mobs.push_back(new_mob.get());
    new_mob->restart();
    return new_mob;
}