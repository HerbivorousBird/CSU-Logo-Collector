#ifndef CUBE_H
#define CUBE_H

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
	unsigned int VBO, VAO;
	Cube()
	{
		setup_vertex();
	}

	~Cube()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void drawCube(glm::ivec3 position, Shader shader)
	{
		glm::mat4 model = glm::mat4(1.0f);
		shader.setVec3f("translate", position);
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

private:
	float ratio = 1;
	void setup_vertex() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	};
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
#endif