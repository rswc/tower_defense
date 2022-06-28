#include "sillyscene.h"
#include "sillyobject.h"
#include "sillyanimatedobject.h"
#include "assimpobject.h"
#include "gamegrid.h"
#include "gridobject.h"
#include "text.h"
#include "skybox.h"

#define PI 3.141592f // close enough

#define cameraMoveSpeed 2.5f
#define mouseSensitivity 0.25f
#define startCameraPosition glm::vec3(0.0f, 3.0f, 3.0f)
#define cameraHeightCap 3.0f
#define pitchLowerBound -70.0f
#define pitchUpperBound -30.0f

SillyScene::SillyScene() {
  activeCamera = RTSCamera(startCameraPosition);
  activeCamera.SetCameraHeightCap(true, cameraHeightCap);

	auto objAssimp = std::make_shared<AssimpObject>();
	Instantiate(std::move(objAssimp));

	// remove if annoying
	auto txt = std::make_shared<Text>("Graphics programming\nis my passion");
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

	gridObj = std::make_shared<GridObject>(
		mesh,
		grid.GetMousePickPlane(),
		grid.GetLogical()
	);

	Instantiate(gridObj);

	Instantiate(std::move(std::make_shared<Skybox>()));

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
		if(key == GLFW_KEY_C) {
			activeCamera.SetCameraHeightCap(true, cameraHeightCap);
			freeFlight = !freeFlight;
			updateCursorState(window);
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

	if (!focus)
		return;
	
	if (doCameraRotation || freeFlight) 
	{
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
		
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

	lastX = xpos;
	lastY = ypos;
}

void SillyScene::OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (!focus)
		{
			firstMouse = true;
			focus = true;
			activeCamera.SetCameraRotationBlock(false); //unblock
			updateCursorState(window);
			return;
		}

		// do mouse pick
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		doCameraRotation = true;
		updateCursorState(window); 
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		doCameraRotation = false;
		updateCursorState(window);
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

void SillyScene::updateCursorState(GLFWwindow* window) {
	if (freeFlight || doCameraRotation)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
}
