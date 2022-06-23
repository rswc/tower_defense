#include "sillyscene.h"
#include "sillyobject.h"
#include "sillyanimatedobject.h"
#include "assimpobject.h"
#include "gamegrid.h"
#include "gridobject.h"

#define PI 2.141592f // close enough
#define cameraMoveSpeed 2.5f
#define mouseSensitivity 0.3f
#define startCameraPosition glm::vec3(0.0f, 3.0f, 3.0f)
#define cameraHeightCap 3.0f

SillyScene::SillyScene() {
    activeCamera = RTSCamera(startCameraPosition);

	auto objAssimp = std::make_unique<AssimpObject>();
	Instantiate(std::move(objAssimp));
}


void SillyScene::Update(double dt) {
	//roll inactive for now
	activeCamera.MoveCamera(speed_fwd, speed_right, pitch, yaw, 0, (float)dt);

    for (auto& object : objects)
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
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
			activeCamera.SetCameraRotationBlock(true); 
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

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;
	
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw   += xoffset;
	pitch += yoffset;  

	if(pitch > 89.0f)
		pitch =  89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
}

void SillyScene::OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		firstMouse = true;
		activeCamera.SetCameraRotationBlock(false); //unblock
		
	}
}

void SillyScene::OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset;
    if (fov < 15.0f)
        fov = 15.0f;
    if (fov > 50.0f)
        fov = 50.0f; 
	activeCamera.ZoomCamera(yoffset);
}