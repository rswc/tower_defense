#include "gameobject.h"


float GameObject::GetOrder(const Camera& camera) const {
    return GetDistanceTo(camera.GetPosition());
}
