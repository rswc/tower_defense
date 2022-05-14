#include "sillyscene.h"
#include "sillyobject.h"


SillyScene::SillyScene() {
    activeCamera = Camera();

    auto obj = std::make_unique<SillyObject>();
    Instantiate(std::move(obj));
}

