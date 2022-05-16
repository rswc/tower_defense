#include "sillyscene.h"
#include "sillyobject.h"
#include "sillyanimatedobject.h"
#define PI 3.141592f

SillyScene::SillyScene() {
    activeCamera = Camera();

    auto obj = std::make_unique<SillyObject>();
    Instantiate(std::move(obj));

    auto objA = std::make_unique<SillyAnimatedObject>();
    Instantiate(std::move(objA));
}

void SillyScene::Update(double dt) {
    auto cam_r = glm::rotate(activeCamera.GetRotation(), (float)dt*speed_x, glm::vec3(1.0f, 0.0f, 0.0f));
    activeCamera.SetRotation(cam_r);
}

void SillyScene::OnKey(GLFWwindow* window, int key, int scancode, int action, int mod) {
    if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			speed_y = -PI;
		}
		if (key == GLFW_KEY_RIGHT) {
			speed_y = PI;
		}
		if (key == GLFW_KEY_UP) {
			speed_x = -PI;
		}
		if (key == GLFW_KEY_DOWN) {
			speed_x = PI;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			speed_y = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			speed_x = 0;
		}
    }
}

