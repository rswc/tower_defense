#include "sillyscene.h"
#include "sillyobject.h"
#include "sillyanimatedobject.h"
#include "assimpobject.h"
#include "text.h"


#define PI 2.141592f // close enough


SillyScene::SillyScene() {
    activeCamera = Camera();
    activeCamera.SetPosition(glm::vec3(0, 1.0f, 2.0f));

	auto objAssimp = std::make_unique<AssimpObject>();
	Instantiate(std::move(objAssimp));

	auto txt = std::make_unique<Text>("helo");
	txt->SetPosition(glm::vec3(20.0f, 20.0f, 0.0f)); // z is unused ¯\_ (ツ)_/¯
	Instantiate(std::move(txt));
}

void SillyScene::Update(double dt) {
    activeCamera.Rotate((float)dt*speed_x, glm::vec3(1.0f, 0.0f, 0.0f));
    activeCamera.Rotate((float)dt*speed_y, glm::vec3(0.0f, 1.0f, 0.0f));

    for (auto& object : objects)
        object->Update(dt);
}

void SillyScene::OnKey(GLFWwindow* window, int key, int scancode, int action, int mod) {
    if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			speed_y = PI;
		}
		if (key == GLFW_KEY_RIGHT) {
			speed_y = -PI;
		}
		if (key == GLFW_KEY_UP) {
			speed_x = PI;
		}
		if (key == GLFW_KEY_DOWN) {
			speed_x = -PI;
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

