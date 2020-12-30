#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class Square
{
public:
	unsigned int VBO, VAO;
	Square() {
		setup_vertices();
	}
	~Square() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void drawHawkeyeSquare(Shader shader,glm::vec3 pos,glm::vec4 color,float aspect)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.95,0.95,0));
		model = glm::scale(model, glm::vec3(0.02,0.02*aspect,1));
		model = glm::translate(model, pos);
		shader.setMat4("model", model);
		shader.setVec4f("acolor", color);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

private:
	void setup_vertices() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
	};

	float vertices[24] = {
		// positions  // texture Coords 
		-1.f,  1.f,  0.0f, 0.0f,
		-1.0f,-1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,

		-1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		1.0f,  1.0f, 1.0f, 0.0f
	};
};

