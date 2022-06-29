#include "sillyscene.h"
#include "sillyobject.h"
#include "animatedobject.h"
#include "gamegrid.h"
#include "gridobject.h"
#include "text.h"

#define PI 2.141592f // close enough

#define cameraMoveSpeed 2.5f
#define mouseSensitivity 0.25f
#define startCameraPosition glm::vec3(0.0f, 3.0f, 3.0f)
#define cameraHeightCap 3.0f
#define pitchLowerBound -70.0f
#define pitchUpperBound -30.0f

SillyScene::SillyScene() {
  activeCamera = RTSCamera(startCameraPosition);
  activeCamera.SetCameraHeightCap(true, cameraHeightCap);

	auto objAssimp = std::make_unique<AnimatedObject>();
	//objAssimp->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
	Instantiate(std::move(objAssimp));

	// remove if annoying
	// auto txt = std::make_unique<Text>("Graphics programming\nis my passion");
	// txt->SetOrigin(glm::vec2(0.02f, 0.5f));
	// txt->SetColor(glm::vec4(1.0f, 0.2f, 0.3f, 0.7f));
	// txt->SetScale(0.8f);
	// Instantiate(std::move(txt));
	
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

	// HACK: We can get away with only doing this at scene init, SO LONG AS:
	//     1) No transparent objects exist in the scene besides Text objects
	//        -> world-space distance to camera does not matter
	//     2) No runtime instantiation of objects occurs
	UpdateDrawOrder();
}


void SillyScene::Update(double dt) {
	//roll inactive for now
	activeCamera.MoveCamera(speed_fwd, speed_right, pitch, yaw, 0, (float)dt);

	for (auto &object : objects)
		object->Update(dt);
}

void SillyScene::OnKey(GLFWwindow* window, int key, int scancode, int action, int mod) {
    if (action == GLFW_PRESS) {
		// if (key == GLFW_KEY_LEFT) {
		// 	pitch = PI;
		// }
		// if (key == GLFW_KEY_RIGHT) {
		// 	pitch = -PI;
		// }
		// if (key == GLFW_KEY_UP) {
		// 	yaw = PI;
		// }
		// if (key == GLFW_KEY_DOWN) {
		// 	yaw = -PI;
		// }
		if(key == GLFW_KEY_C) {
			activeCamera.SetCameraHeightCap(true, cameraHeightCap);
			freeFlight = !freeFlight;
		}
		if (key == GLFW_KEY_W)
		{
			speed_fwd = cameraMoveSpeed;
		}
		if (key == GLFW_KEY_S)
		{
			speed_fwd = -cameraMoveSpeed;
		}
		if (key == GLFW_KEY_D)
		{
			speed_right = cameraMoveSpeed;
		}
		if (key == GLFW_KEY_A)
		{
			speed_right = -cameraMoveSpeed;
		}
		if (key == GLFW_KEY_ESCAPE)
		{
			focus = false;
			activeCamera.SetCameraRotationBlock(true); 
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
		}
		
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			pitch = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			yaw = 0;
		}
		if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
			speed_fwd = 0;
		}
		if (key == GLFW_KEY_D || key == GLFW_KEY_A) {
			speed_right = 0;
		}
	}
}

void SillyScene::OnMouse(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

	if (focus == true) 
	{
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
		lastX = xpos;
		lastY = ypos;
		
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw   += xoffset;
		pitch += yoffset;  

		if(freeFlight == false)
		{
			if(pitch > pitchUpperBound)
				pitch =  pitchUpperBound;
			if(pitch < pitchLowerBound)
				pitch = pitchLowerBound;
		}
	}
}

void SillyScene::OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		firstMouse = true;
		focus = true;
		activeCamera.SetCameraRotationBlock(false); //unblock
	}
}

void SillyScene::OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset * 5;
    if (fov < 30.0f)
        fov = 30.0f;
    if (fov > 90.0f)
        fov = 90.0f; 
	activeCamera.ZoomCamera(fov);
}
