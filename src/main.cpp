#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <memory>
#include "GlobalConfig.h"
#include "resources.h"
#include "sillyscene.h"
#include "sillyobject.h"
#include "shaderprogram.h"


#include "grid_test.h"

std::unique_ptr<Scene> scene;

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod) {
	scene->OnKey(window, key, scancode, action, mod);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	scene->OnMouse(window, xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	scene->OnMouseButton(window, button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	scene->OnScroll(window, xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
	if (scene){
		scene->SetScreenSize((float)width, (float)height);
		GlobalConfig::windowWidth = (float)width;
		GlobalConfig::windowHeight = (float)height;
	}
}

void runTests() {
	// Run Test for logical Grid class
	GridTest();
}

int main(void)
{
	runTests();
	GlobalConfig::loadConfigFromFile("config.txt");
	GlobalConfig::printWholeConfig();
	
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(GlobalConfig::windowWidth, GlobalConfig::windowHeight, "OpenGL", NULL, NULL);
	glViewport(0, 0, GlobalConfig::windowWidth, GlobalConfig::windowHeight);

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	initShaders();

	Resources::Initialize();

	scene = std::make_unique<SillyScene>();
	scene->SetScreenSize(GlobalConfig::windowWidth, GlobalConfig::windowHeight);

	glfwSetKeyCallback(window, key_callback);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	glfwSetTime(0);
	double dt = 0.0;

	//Główna pętla
	while (!glfwWindowShouldClose(window))
	{
		dt = glfwGetTime();
		glfwSetTime(0);

		scene->Update(dt);

		if (scene->IsTransitionInitiated())
		{
			auto progenitor = std::move(scene);

			scene = progenitor->GetTransitionTarget();

			int width, height;
			glfwGetWindowSize(window, &width, &height);
			scene->SetScreenSize(width, height);
		}	

        scene->Draw();

        glfwSwapBuffers(window);
		glfwPollEvents();
	}

	freeShaders();
	Resources::Free();
	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
