#include "scene.h"

#include <GLFW/glfw3.h>
#include <algorithm>


void Scene::Update(double dt) {
    for (auto& object : objects)
        object->Update(dt);
}

void Scene::Draw() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& object : objects)
        object->Draw(activeCamera);
}

void Scene::OnKey(GLFWwindow* window, int key, int scancode, int action, int mod) {
    // ???
}

void Scene::OnMouse(GLFWwindow* window, double xpos, double ypos) {
    // ???
}
void Scene::OnMouseButton(GLFWwindow* window, int button, int action, int mods) {
    // ???
}

void Scene::OnScroll(GLFWwindow* window, double xoffset, double yoffset) {
    // ???
}

void Scene::Instantiate(std::shared_ptr<GameObject> object) {
    objects.push_back(std::move(object));
}

void Scene::SetScreenSize(float width, float height) {
    activeCamera.SetScreenSize(width, height);
}

void Scene::UpdateDrawOrder() {
    RTSCamera& camera = activeCamera;

    std::sort(
        objects.begin(),
        objects.end(),
        [camera](const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b) -> bool {
            return a->GetOrder(camera) > b->GetOrder(camera);
        }
    );
}

