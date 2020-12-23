#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "resource_manager.h"
#include "camera.h"
#include "Cube.h"
#include "PhysicsEngine.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
int initWindow();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
GLFWwindow* window;

// camera
Camera camera(glm::vec3(3.0f, 10.0f, 3.0f));
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

	ResourceManager::LoadShader("cube_shader.vs", "cube_shader.fs", nullptr, "cube");
	ResourceManager::LoadTexture("resources/textures/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("resources/textures/container.jpg", false, "con");
	unsigned int cubemapTexture = ResourceManager::loadCubeMapFromFile("resources/textures/skybox/");
	ResourceManager::LoadShader("6.1.skybox.vs", "6.1.skybox.fs", nullptr, "skyboxShader");
	Shader ourShader = ResourceManager::GetShader("cube");
	auto face = ResourceManager::GetTexture("face");
	auto con = ResourceManager::GetTexture("con");
	Shader skyboxShader = ResourceManager::GetShader("skyboxShader");

	int maps[36][12] = {
		1,1,2,2,3,1,1,1,2,2,3,1,
		2,2,1,2,2,1,2,2,1,2,2,1,
		3,2,2,1,2,1,3,2,2,1,2,1,
		4,3,3,2,2,2,4,3,3,2,2,2,
		3,4,3,3,2,1,3,4,3,3,2,1,
		3,3,2,3,2,2,3,3,2,3,2,2,
		1,1,2,2,3,1,1,1,2,2,3,1,
		2,2,1,2,2,1,2,2,1,2,2,1,
		3,2,2,1,2,1,3,2,2,1,2,1,
		4,3,3,2,2,2,4,3,3,2,2,2,
		3,4,3,3,2,1,3,4,3,3,2,1,
		3,3,2,3,2,2,3,3,2,3,2,2,
		1,1,2,2,3,1,1,1,2,2,3,1,
		2,2,1,2,2,1,2,2,1,2,2,1,
		3,2,2,1,2,1,3,2,2,1,2,1,
		4,3,3,2,2,2,4,3,3,2,2,2,
		3,4,3,3,2,1,3,4,3,3,2,1,
		3,3,2,3,2,2,3,3,2,3,2,2,
		1,1,2,2,3,1,1,1,2,2,3,1,
		2,2,1,2,2,1,2,2,1,2,2,1,
		3,2,2,1,2,1,3,2,2,1,2,1,
		4,3,3,2,2,2,4,3,3,2,2,2,
		3,4,3,3,2,1,3,4,3,3,2,1,
		3,3,2,3,2,2,3,3,2,3,2,2,
		1,1,2,2,3,1,1,1,2,2,3,1,
		2,2,1,2,2,1,2,2,1,2,2,1,
		3,2,2,1,2,1,3,2,2,1,2,1,
		4,3,3,2,2,2,4,3,3,2,2,2,
		3,4,3,3,2,1,3,4,3,3,2,1,
		3,3,2,3,2,2,3,3,2,3,2,2,
		1,1,2,2,3,1,1,1,2,2,3,1,
		2,2,1,2,2,1,2,2,1,2,2,1,
		3,2,2,1,2,1,3,2,2,1,2,1,
		4,3,3,2,2,2,4,3,3,2,2,2,
		3,4,3,3,2,1,3,4,3,3,2,1,
		3,3,2,3,2,2,3,3,2,3,2,2
	};


	auto cube = new Cube();
	pEngine->initBoundary(&maps[0][0], 36, 12);
	ourShader.use();
	ourShader.setInt("texture1", 0);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();


		pEngine->updateCameraVertMovement(lastPos, camera.Position, deltaTime);
		lastPos = camera.Position;


		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		
		glBindVertexArray(cube->VAO);
		glActiveTexture(GL_TEXTURE0);
		con.Bind();
		// render boxes
		for (unsigned int i = 0; i < 36; i++)
		{
			for (unsigned int j = 0; j < 12; j++) {
				int z = maps[i][j];
				for (unsigned int k = 0; k < z; k++) {
					auto po = glm::vec3(i, k, j);
					cube->drawCube(po, ourShader);
				}
			}
		}

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(cube->VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
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
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
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
}