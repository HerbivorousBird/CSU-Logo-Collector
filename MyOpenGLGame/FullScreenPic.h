#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class FullScreenPic
{
public:
	unsigned int VBO, VAO;
	FullScreenPic() {
		setup_vertices();
	}
	~FullScreenPic() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void drawPic(Shader shader)
	{
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	};

	float vertices[30] = {
		// positions  // texture Coords 
		-1.f,  1.f,  0.0f, 0.0f, 0.0f,
		-1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,

		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 0.0f
	};
};

