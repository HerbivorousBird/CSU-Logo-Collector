#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "Texture.h"
#include "GameMap.h"

class Cube
{
public:
	Cube(GameMap* map);
	~Cube();
	
	void BindBuffer(Texture tex);
	unsigned int VBO, VAO;
	unsigned int instanceVBO;
	void drawCube(glm::ivec3 position, Shader shader);
	void drawSkybox(Shader shader);

private:

	float ratio = 1;
	void setup_vertex();
	GameMap *map;
	float vertices[180] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  ratio, 0.0f,
	 0.5f,  0.5f, -0.5f,  ratio, ratio,
	 0.5f,  0.5f, -0.5f,  ratio, ratio,
	-0.5f,  0.5f, -0.5f,  0.0f, ratio,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  ratio, 0.0f,
	 0.5f,  0.5f,  0.5f,  ratio, ratio,
	 0.5f,  0.5f,  0.5f,  ratio, ratio,
	-0.5f,  0.5f,  0.5f,  0.0f, ratio,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  ratio, 0.0f,
	-0.5f,  0.5f, -0.5f,  ratio, ratio,
	-0.5f, -0.5f, -0.5f,  0.0f, ratio,
	-0.5f, -0.5f, -0.5f,  0.0f, ratio,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  ratio, 0.0f,

	 0.5f,  0.5f,  0.5f,  ratio, 0.0f,
	 0.5f,  0.5f, -0.5f,  ratio, ratio,
	 0.5f, -0.5f, -0.5f,  0.0f, ratio,
	 0.5f, -0.5f, -0.5f,  0.0f, ratio,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  ratio, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, ratio,
	 0.5f, -0.5f, -0.5f,  ratio, ratio,
	 0.5f, -0.5f,  0.5f,  ratio, 0.0f,
	 0.5f, -0.5f,  0.5f,  ratio, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, ratio,

	-0.5f,  0.5f, -0.5f,  0.0f, ratio,
	 0.5f,  0.5f, -0.5f,  ratio, ratio,
	 0.5f,  0.5f,  0.5f,  ratio, 0.0f,
	 0.5f,  0.5f,  0.5f,  ratio, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, ratio
	};
};


