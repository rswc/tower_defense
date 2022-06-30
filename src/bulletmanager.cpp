#include "bulletmanager.h"
#include "scene.h"

#include <vector>
#include <iostream>

int BulletManager::countFreeBullets() {
    int cnt = 0;
    for (auto ptr_bullet : bullets) {
        if (!ptr_bullet->active) cnt++;
    }
    return cnt;
}
void BulletManager::reactivateBullet(glm::vec3 src, glm::vec3 dst, float time) {
    for (auto ptr_bullet : bullets) {
        if (!ptr_bullet->active) {
            ptr_bullet->restart(src, dst, time);
            return;
        }
    }
} 

std::unique_ptr<BulletObject> BulletManager::createBullet(glm::vec3 src, glm::vec3 dst, float time) {
    auto ptr_bullet = std::make_unique<BulletObject>();
    ptr_bullet->restart(src, dst, time);
    bullets.push_back(ptr_bullet.get());
    return ptr_bullet;
}

void BulletManager::shootBullet(glm::vec3 src, glm::vec3 dst, float time) {
    // std::cout << "Spawning new Bullet that will hit: " << dst.x << " " << dst.y << " " << dst.z << " after " << time << " from " << src.x << " " << src.y << " " << src.z << std::endl;
    if (countFreeBullets() > 0) {
        reactivateBullet(src, dst, time);
    } else {
        assert(scene_ptr && "scene pointer is null in BulletManager");
        auto ptr = createBullet(src, dst, time);
        scene_ptr->Instantiate(std::move(ptr));
    }
}


