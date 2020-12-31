
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "resource_manager.h"
#include "camera.h"
#include "Cube.h"
#include "CSUlogo.h"
#include "PhysicsEngine.h"
#include "GameMap.h"
#include <iostream>
#include "GameSceneRender.h"
#include "ParticleSystem.h"

void esc_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
int initWindow();

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
float aspect = ((float)SCR_WIDTH) / SCR_HEIGHT;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool mouseIsCaptured = true;


GLFWwindow* window;
Camera* camera;
PhysicsEngine* pEngine;
GameMap* gmap;
GameSceneRender* gameSceneP;
Scene* currentScene;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
	initWindow();
	GameSceneRender gameScene;
	gameSceneP = &gameScene;
	gameScene.initResourse();
	gameScene.bindTex2Sha();
	currentScene = &gameScene.currentScene;
	pEngine = &gameScene.pEngine;
	camera = &gameScene.camera;
	gmap = &gameScene.gmap;
	gameScene.initDefaultMap();
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (*currentScene) {
		case MAINMENU:
		case HELP_C:
		case HELP_M:
		case WIN:
			gameScene.updataPara(aspect, deltaTime);
			gameScene.DrawMap();
			gameScene.DrawParticle();
			gameScene.DrawSkybox();
			gameScene.DrawFullScreen();
			break;

		case COLLECT:
			gameScene.updataPara(aspect, deltaTime);
			gameScene.DrawMap();
			gameScene.DrawLogo();
			gameScene.DrawCollectScore(aspect);
			gameScene.DrawParticle();
			gameScene.DrawSkybox();
			if (pEngine->isCollectable()) {
				gameScene.collectScore++;
				gameScene.particle.init(60, gmap->targetLoc);
				if (gameScene.collectScore >= 5) {
					*currentScene = WIN;
				}
				gmap->randTarget();
			}
			break;
		case MAZE:
			gameScene.updataPara(aspect, deltaTime);
			gameScene.DrawMap();
			gameScene.DrawLogo();
			gameScene.DrawHawkeye(aspect);
			gameScene.DrawParticle();
			gameScene.DrawSkybox();
			if (pEngine->isCollectable()) {
				gameScene.particle.init(60, gmap->targetLoc);
				*currentScene = WIN;
				gmap->randTarget();
			}
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	switch (*currentScene) {
	case MAINMENU:
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			*currentScene = HELP_C;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			*currentScene = HELP_M;
		gameSceneP->initMap();
		break;
	case HELP_C:
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			*currentScene = COLLECT;
		}
		break;
	case HELP_M:
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			*currentScene = MAZE;
		}
		break;
	case WIN:
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			*currentScene = MAINMENU;
		break;
	default:
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			pEngine->jump();
	}
}

void esc_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key== GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
		switch (*currentScene)
		{
		case MAINMENU:
			glfwSetWindowShouldClose(window, true);
			break;
		default:
			*currentScene = MAINMENU;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
		if (mouseIsCaptured)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mouseIsCaptured = !mouseIsCaptured;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	aspect = height > 0 ? width / (double)height : 1.0;
	glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;
	camera->ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
}

int initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// -------------------

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSU Logo Collector", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 获取第一个显示器分辨率，从而设置窗口居中
	int monitorCount;
	GLFWmonitor** pMonitor = glfwGetMonitors(&monitorCount);
	const GLFWvidmode* mode = glfwGetVideoMode(*pMonitor);
	glfwSetWindowPos(window, (mode->width - SCR_WIDTH) / 2, (mode->height - SCR_HEIGHT) / 2);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, esc_callback);
	glfwSwapInterval(1); //打开垂直同步

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
}

// Release版本改为win32程序后入口。
int WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
	main();
}