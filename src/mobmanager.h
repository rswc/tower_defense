#pragma once

#include "mobobject.h"

#include <vector>
#include <memory>

class MobManager {
    friend class Tower;

    int nextMobId = 0;

    std::vector<MobObject*> mobs;
public: 
    MobManager() = default;
    ~MobManager() = default;

    int countFreeMobs();
    void reactivateMob(); 
    std::unique_ptr<MobObject> createMob(GameGrid* gameGridPtr);
};
