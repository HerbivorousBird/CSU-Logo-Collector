#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class CSUlogo
{
public:
	CSUlogo();
	unsigned int VBO, VAO;
	void setup_vertex();
	void updateAngle();
	void drawLogo(glm::vec3 position, Shader shader);

private:
	float vertices[30] = {
		// positions          // texture Coords 
		-0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.0f,  1.0f,  1.0f,

		-0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	int angle;
};

