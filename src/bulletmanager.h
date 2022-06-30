#pragma once

#include "bulletobject.h"
class Scene;

class BulletManager {
    std::vector<BulletObject*> bullets;
    Scene * scene_ptr = nullptr;

    int countFreeBullets();
    void reactivateBullet(glm::vec3 src, glm::vec3 dst, float time); 
    std::unique_ptr<BulletObject> createBullet(glm::vec3 src, glm::vec3 dst, float time);
public: 
    BulletManager(Scene * scene) : bullets(), scene_ptr(scene) {}
    void shootBullet(glm::vec3 src, glm::vec3 dst, float time);
};
