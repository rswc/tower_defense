#include "sillyscene.h"
#include "sillyobject.h"
#include "sillyanimatedobject.h"
#include "assimpobject.h"
#include "gridobject.h"
#include "mobobject.h"
#include "text.h"
#include "skybox.h"

#define PI 3.141592f // close enough

#define cameraMoveSpeed 2.5f
#define mouseSensitivity 0.25f
#define startCameraPosition glm::vec3(0.0f, 3.0f, 3.0f)
#define cameraHeightCap 3.0f
#define pitchLowerBound -70.0f
#define pitchUpperBound -30.0f

// Add arguments if necessary
void SillyScene::SceneTransition() {
	// Save any parameters of the target scene here

	// ALWAYS call super at the end
	Scene::SceneTransition();
}

SillyScene::SillyScene() {
  activeCamera = RTSCamera(startCameraPosition);
  activeCamera.SetCameraHeightCap(true, cameraHeightCap);

	// auto objAssimp = std::make_shared<AssimpObject>();
	// Instantiate(std::move(objAssimp));


	// remove if annoying
	auto txt = std::make_shared<Text>("Graphics programming\nis my passion");
	txt->SetOrigin(glm::vec2(0.02f, 0.5f));
	txt->SetColor(glm::vec4(1.0f, 0.2f, 0.3f, 0.7f));
	txt->SetScale(0.8f);
	Instantiate(std::move(txt));
	
	std::vector<std::string> map {{ "xxxxxx",
                "xS...x",
                "xxxx.x",
                "x....x",
                "x.xxxx",
                "x.x.Ex",
                "x.x.xx",
                "x...xx",
                "xxxxxx"
            }};

	grid = std::make_unique<GameGrid>(map);

	// GameGrid grid({{{"xxx", "xSx", "x.x", "xEx", "xxx"}}});
	GameGrid::GameGridMesh mesh = grid->generateBaseMesh(GameGrid::MESH_V_SECOND);
	std::cerr << "n of mesh vertices: " << mesh.vertices.size() << std::endl;

	gridObj = std::make_shared<GridObject>(
		mesh,
		grid->GetMousePickPlane(),
		grid->GetLogical()
	);

	Instantiate(gridObj);

	Instantiate(std::move(std::make_shared<Skybox>()));

	// HACK: We can get away with only doing this at scene init, SO LONG AS:
	//     1) No transparent objects exist in the scene besides Text objects
	//        -> world-space distance to camera does not matter
	//     2) No runtime instantiation of objects occurs
	UpdateDrawOrder();
}


std::unique_ptr<Scene> SillyScene::GetTransitionTarget() const {
	// If necessary, use different Scene subtype
	auto scn = std::make_unique<SillyScene>();
	
	// Apply saved parameters

	return scn;
}

void SillyScene::Update(double dt) {
	//roll inactive for now
	activeCamera.MoveCamera(speed_fwd, speed_right, pitch, yaw, 0, (float)dt);

	Scene::Update(dt);
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
		
		if (key == GLFW_KEY_M) {
			
			if (mobManager.countFreeMobs() > 0) {
				mobManager.reactivateMob();
			} else {
				Instantiate(std::move(mobManager.createMob(grid.get())));
			}
		}
		
		if (key == GLFW_KEY_X) {
			SceneTransition();
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
		Ray ray = activeCamera.ViewportToRay(lastX, lastY);
		glm::vec3 hit;
		if (ray.Intersect(gridObj->GetMousePickPlane(), hit))
		{
			std::cerr << "Hit: X: " << hit.x << " Y: " << hit.y << " Z: " << hit.z << std::endl;
			
			// gridObj->GetLogical()
			// world to grid position
			// Check money/whatever
			// Place tower
		}
		else
		{
			std::cerr << "No hit!\n";
		}
		

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
