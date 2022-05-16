#include "sillyscene.h"
#include "sillyobject.h"
#include "sillyanimatedobject.h"


SillyScene::SillyScene() {
    activeCamera = Camera();

    auto obj = std::make_unique<SillyObject>();
    Instantiate(std::move(obj));

    auto objA = std::make_unique<SillyAnimatedObject>();
    Instantiate(std::move(objA));
}

