#include "sillyscene.h"
#include "sillyobject.h"
#include "sillyanimatedobject.h"
#include "assimpobject.h"

#include "gamegrid.h"
#include "gridobject.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>



#define PI 2.141592f // close enough


SillyScene::SillyScene() {
    activeCamera = Camera();
    activeCamera.SetPosition(glm::vec3(3, 3.0f, 10.0f));

	// auto objAssimp = std::make_unique<AssimpObject>();
	// Instantiate(std::move(objAssimp));
	
	
            GameGrid grid({{{
                "xxxxxx",
                "xS...x",
                "xxxx.x",
                "x....x",
                "x.xxxx",
                "x.x.Ex",
                "x.x.xx",
                "x...xx",
                "xxxxxx"
            }}});
	// GameGrid grid({{{"xxx", "xSx", "x.x", "xEx", "xxx"}}});
	BaseMesh mesh = grid.generateBaseMesh(GameGrid::MESH_V_FIRST);
	std::cerr << "n of mesh vertices: " << mesh.vertices.size() << std::endl;
	auto objGrid = std::make_unique<GridObject>(mesh);
	Instantiate(std::move(objGrid));
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

