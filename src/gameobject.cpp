#include "gameobject.h"


GameObject::DrawOrderPosition GameObject::GetOrder(const Camera& camera) const {
    return { LAYER_OPAQUE, GetDistanceTo(camera.GetPosition()) };
}
