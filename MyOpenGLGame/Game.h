#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"

class Game
{
	// camera
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;
	GLuint Width, Height;
	GLFWwindow* window;
	// Constructor/Destructor
	Game(GLuint width, GLuint height, GLFWwindow* window);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();

	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
};
