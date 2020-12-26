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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
int initWindow();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float aspect = ((float)SCR_WIDTH) / SCR_HEIGHT;
GLFWwindow* window;

// camera
Camera camera(glm::vec3(3.0f, 5.0f, 3.0f));
PhysicsEngine* pEngine = new PhysicsEngine();
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{

	initWindow();

	GameMap* gamemap1 = new GameMap();
	gamemap1->initFromTxt("resources/maps/map1.txt");
	int* maps = gamemap1->mapArr;
	camera.Position = gamemap1->roalPos;

	ResourceManager::LoadShader("resources/shader/cube_shader.vs", "resources/shader/cube_shader.fs", nullptr, "cube");
	ResourceManager::LoadShader("resources/shader/logo_shader.vs", "resources/shader/logo_shader.fs", nullptr, "logo");
	ResourceManager::LoadTexture2D("resources/textures/brickwall.jpg",false,"wallTex");
	ResourceManager::LoadTexture2D("resources/textures/透明校徽2.png",true,"logoTex");
	ResourceManager::LoadTextureCube("resources/textures/skybox/","skyboxTex");
	ResourceManager::LoadTextureCube("resources/textures/grassland/","grasslandTex",GL_NEAREST);
	ResourceManager::LoadShader("resources/shader/skybox.vs", "resources/shader/skybox.fs", nullptr, "skyboxShader");
	Shader ourShader = ResourceManager::GetShader("cube");
	auto wallTex = ResourceManager::GetTexture("wallTex");
	auto logoTex = ResourceManager::GetTexture("logoTex");
	auto skyboxTex = ResourceManager::GetTexture("skyboxTex");
	auto grasslandTex = ResourceManager::GetTexture("grasslandTex");
	auto logoShader = ResourceManager::GetShader("logo");
	Shader skyboxShader = ResourceManager::GetShader("skyboxShader");
	auto tex = ResourceManager::LoadTextureCube("resources/textures/grassland/", "grasslandTex", GL_NEAREST);


	auto cube = new Cube(gamemap1);
	auto logo = new CSUlogo();

	pEngine->initBoundary(maps, gamemap1->mapx, gamemap1->mapz);
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	logoShader.use();
	logoShader.setInt("texture1", 0);

	glm::vec3 lastPos = camera.Position;

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		pEngine->updateCameraVertMovement(lastPos, camera.Position, deltaTime);
		logo->updateAngle();
		lastPos = camera.Position;
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),aspect, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();


		ourShader.use();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		glBindVertexArray(cube->VAO);
		glActiveTexture(GL_TEXTURE0);
		grasslandTex.Bind();

		// render boxes
		for (unsigned int i = 0; i < gamemap1->mapz; i++)
		{
			for (unsigned int j = 0; j < gamemap1->mapx; j++) {
				int z = maps[i*gamemap1->mapx+j];
				for (unsigned int k = 0; k < z; k++) {
					auto po = glm::ivec3(j, k, i);
					cube->drawCube(po, ourShader);
				}
			}
		}

		logoShader.use();
		logoShader.setMat4("projection", projection);
		logoShader.setMat4("view", view);
		glBindVertexArray(logo->VAO);
		glActiveTexture(GL_TEXTURE0);
		logoTex.Bind();
		logo->drawLogo(gamemap1->roalPos+glm::ivec3(0,0,1), logoShader);

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(cube->VAO);
		glActiveTexture(GL_TEXTURE0);
		skyboxTex.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete cube;
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		pEngine->jumpAndUpdateVelocity();
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
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
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyGame", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
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

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0);
}