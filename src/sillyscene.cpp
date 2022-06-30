#include "sillyscene.h"
#include "sillyobject.h"
#include "animatedobject.h"
#include "gamegrid.h"
#include "gridobject.h"
#include "mobobject.h"
#include "text.h"
#include "skybox.h"
#include "GlobalConfig.h"
#include "tower.h"
#include "StaticObject.h"


#define PI 3.141592f // close enough
#define pitchLowerBound -70.0f
#define pitchUpperBound -30.0f

// Add arguments if necessary
void SillyScene::SceneTransition() {
	// Save any parameters of the target scene here

	// ALWAYS call super at the end
	Scene::SceneTransition();
}

SillyScene::SillyScene(int mapID) : currentMap(mapID), bulletManager(this) {
	//Override with data from config file
	cameraMoveSpeed = GlobalConfig::cameraMoveSpeed;
	mouseSensitivity = GlobalConfig::mouseSensitivity;
	cameraHeightCap = GlobalConfig::cameraHeightCap;
	startCameraPosition.x = GlobalConfig::cameraStartX;
	startCameraPosition.z = GlobalConfig::cameraStartZ;
	pitch = GlobalConfig::cameraStartAngle;

	std::cout<<"Camera start position"<<startCameraPosition.x<<" "<<startCameraPosition.z<<std::endl;
	activeCamera = RTSCamera(startCameraPosition);
	activeCamera.SetCameraHeightCap(true, cameraHeightCap);

	std::cerr << "Loading SillyScene with map #" << currentMap << std::endl;

	// auto objAssimpAnimated = std::make_unique<AnimatedObject>(glm::vec3(0.03f, 0.03f, 0.03f), 1.0f);
	// objAssimpAnimated->SetRotation(glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)));
	// objAssimpAnimated->SetPosition(glm::vec3(2.0f, 0.2f, 0.0f));
 	// Instantiate(std::move(objAssimpAnimated));

	auto objStatic1 = std::make_unique<StaticObject>(
		"assets/gate3/gate3.fbx", 
		"assets/gate3/gate3.png", 
		"assets/gate3/gate3.occlusion.png");
	objStatic1->SetScale(glm::vec3(0.0013f, 0.0013f, 0.0013f));
	objStatic1->SetRotation(glm::quat(glm::vec3(-1.57079633f, 0.0f, 0.0f)));
	objStatic1->SetPosition(glm::vec3(1.50f, 0.0f, 1.27f));
	Instantiate(std::move(objStatic1));

	auto objStatic2 = std::make_unique<StaticObject>(
		"assets/gate3/gate3.fbx", 
		"assets/gate3/gate3.png", 
		"assets/gate3/gate3.occlusion.png");
	objStatic2->SetScale(glm::vec3(0.0013f, 0.0013f, 0.0013f));
	objStatic2->SetRotation(glm::quat(glm::vec3(-PI/2, PI, 0.0f)));
	objStatic2->SetPosition(glm::vec3(5.52f, 0.0f, 4.70f));
	Instantiate(std::move(objStatic2));

	// auto objAssimp = std::make_shared<AssimpObject>();
	// Instantiate(std::move(objAssimp));

	// remove if annoying
	auto txt = std::make_shared<Text>("Graphics programming\nis my passion");
	txt->SetOrigin(glm::vec2(0.02f, 0.5f));
	txt->SetColor(glm::vec4(1.0f, 0.2f, 0.3f, 0.7f));
	txt->SetScale(0.8f);
	Instantiate(std::move(txt));
	
	/*
	auto rockObj = std::make_shared<BulletObject>();
	rockObj->restart(glm::vec3(0,0,0), glm::vec3(0,0,0), 100.f);
	Instantiate(std::move(rockObj));
	*/

	
	std::vector<std::string> map {{ "xxxxxx",
                "xS...x",
                "@xxx.x",
                "x....x",
                "x.xxxx",
                "x.x.Ex",
                "x.@.xx",
                "x...xx",
                "xxxx@x"
            }};

	grid = std::make_unique<GameGrid>(map);
	
	std::cout << "GameStart: " << grid->getStartPoint().x << " " << grid->getStartPoint().z << std::endl;
	std::cout << "GameExit: " << grid->getExitPoint().x << " " << grid->getExitPoint().z << std::endl;

	// GameGrid grid({{{"xxx", "xSx", "x.x", "xEx", "xxx"}}});
	GameGrid::GameGridMesh mesh = grid->generateBaseMesh(GameGrid::MESH_V_SECOND);
	std::cerr << "n of mesh vertices: " << mesh.vertices.size() << std::endl;

	gridObj = std::make_shared<GridObject>(
		mesh,
		grid->GetMousePickPlane(),
		grid->GetLogical()
	);

	Instantiate(gridObj);



	activeCamera.PushLight(Camera::PointLight(
		grid->getStartPoint() + glm::vec3(0.f, .6f, 0.f),
		{0.05f, 0.05f, 0.05f},
		{0.9f, 0.8f, 0.4f},
		{1.0f, 0.9f, 0.6f},
		1.2f, 0.6f, 1.f
	));

	Instantiate(std::move(std::make_shared<Skybox>()));

	for (auto& gp : gridObj->GetLogical().GetTreePositions())
	{
		// auto tree = std::make_shared<...>();
		// tree->SetPosition(gridObj->GridToWorld(gp));
		// Instantiate(tree);
		auto objAssimpAnimated1 = std::make_shared<AnimatedObject>(glm::vec3(0.02f, 0.02f, 0.02f), 1.0f);
		objAssimpAnimated1->SetRotation(glm::quat(glm::vec3(0.0f, (float)(rand() % 6), 0.0f)));
		objAssimpAnimated1->SetPosition(gridObj->GridToWorld(gp));
		Instantiate(objAssimpAnimated1);
	}
	

	// HACK: We can get away with only doing this at scene init, SO LONG AS:
	//     1) No transparent objects exist in the scene besides Text objects
	//        -> world-space distance to camera does not matter
	//     2) No runtime instantiation of objects occurs
	UpdateDrawOrder();
}


std::unique_ptr<Scene> SillyScene::GetTransitionTarget() const {
	// If necessary, use different Scene subtype
	auto scn = std::make_unique<SillyScene>((currentMap + 1) % numAvailableMaps);
	
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
			auto gp = gridObj->WorldToGrid(hit);
			std::cerr << "Hit: R: " << gp.row << " C: " << gp.col <<  std::endl;
			
			// Check money/whatever
			// 
			if (mods != GLFW_MOD_SHIFT && gridObj->GetLogical().TryPlaceTower(gp))
			{
				if (towerManager.countFreeTowers() > 0) {
					towerManager.reactivateTower(gridObj->GridToWorld(gp) + grid->getHeightVector());
					std::cout << "Pool tower placed!" << std::endl;
				} else {
					auto tower =  towerManager.createTower(&bulletManager, &mobManager, gridObj->GridToWorld(gp) + grid->getHeightVector());
					std::cout << "New tower placed! (" << tower->GetPosition().x << " " << tower->GetPosition().z << ")" << std::endl; 
					Instantiate(std::move(tower));
				}
			}
			if (mods == GLFW_MOD_SHIFT && gridObj->GetLogical().TryTakeTower(gp)) {
				towerManager.deactivateTower(gridObj->GridToWorld(gp));
				std::cout << "Pool tower taken down!" << std::endl;
			}
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
