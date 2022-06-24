#include "sillyscene.h"
#include "sillyobject.h"
#include "sillyanimatedobject.h"
#include "assimpobject.h"
#include "gamegrid.h"
#include "gridobject.h"
#include "text.h"

#define PI 2.141592f // close enough

SillyScene::SillyScene()
{
	activeCamera = Camera();
	activeCamera.SetPosition(glm::vec3(0, 1.0f, 2.0f));

	auto objAssimp = std::make_unique<AssimpObject>();
	Instantiate(std::move(objAssimp));

	// remove if annoying
	auto txt = std::make_unique<Text>("Graphics programming\nis my passion");
	txt->SetOrigin(glm::vec2(0.02f, 0.5f));
	txt->SetColor(glm::vec4(1.0f, 0.2f, 0.3f, 0.7f));
	txt->SetScale(0.8f);
	Instantiate(std::move(txt));
	
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
	GameGrid::GameGridMesh mesh = grid.generateBaseMesh(GameGrid::MESH_V_SECOND);
	std::cerr << "n of mesh vertices: " << mesh.vertices.size() << std::endl;
	auto objGrid = std::make_unique<GridObject>(mesh);
	Instantiate(std::move(objGrid));
}

void SillyScene::Update(double dt) {
    // activeCamera.Rotate((float)dt*speed_yaw, glm::vec3(1.0f, 0.0f, 0.0f));
    // activeCamera.Rotate((float)dt*speed_pitch, glm::vec3(0.0f, 1.0f, 0.0f));
	activeCamera.CumulativeRotation((float)dt*speed_yaw, glm::vec3(1.0f, 0.0f, 0.0f));
    activeCamera.CumulativeRotation((float)dt*speed_pitch, glm::vec3(0.0f, 1.0f, 0.0f));

	activeCamera.SetPosition(activeCamera.GetPosition() + (float)dt * speed_fwd * activeCamera.GetForward());
	activeCamera.SetPosition(activeCamera.GetPosition() + (float)dt * speed_right * activeCamera.GetRight());

	for (auto &object : objects)
		object->Update(dt);
}

void SillyScene::OnKey(GLFWwindow* window, int key, int scancode, int action, int mod) {
    if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			speed_pitch = PI;
		}
		if (key == GLFW_KEY_RIGHT) {
			speed_pitch = -PI;
		}
		if (key == GLFW_KEY_UP) {
			speed_yaw = PI;
		}
		if (key == GLFW_KEY_DOWN) {
			speed_yaw = -PI;
		}
		if (key == GLFW_KEY_W)
		{
			speed_fwd = 2.0f;
		}
		if (key == GLFW_KEY_S)
		{
			speed_fwd = -2.0f;
		}
		if (key == GLFW_KEY_D)
		{
			speed_right = 2.0f;
		}
		if (key == GLFW_KEY_A)
		{
			speed_right = -2.0f;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			speed_pitch = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			speed_yaw = 0;
		}
		if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
			speed_fwd = 0;
		}
		if (key == GLFW_KEY_D || key == GLFW_KEY_A) {
			speed_right = 0;
		}
	}
}
