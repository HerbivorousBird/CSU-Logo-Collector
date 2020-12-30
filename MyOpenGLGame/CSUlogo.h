#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class CSUlogo
{
public:
	unsigned int VBO, VAO;
	CSUlogo() {
		setup_vertices();
	};
	~CSUlogo() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void updateAngle()
	{
		angle = angle++ % 360;
	}

	void drawLogo(glm::vec3 position, Shader shader,glm::vec3 viewPos)
	{
		shader.setVec3f("viewPos", viewPos);
		shader.setFloat("aalpha", 0.8);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(float(angle)), glm::vec3(0.0, 1, 0.0));
		model = glm::scale(model, glm::vec3(0.7));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void drawParticleLogo(glm::vec3 position, Shader shader,float alpha,int angle) {
		shader.setVec3f("viewPos", glm::vec3(1, 1, 1));
		shader.setFloat("aalpha", alpha);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(float(angle)), glm::vec3(0.0, 1, 0.0));
		model = glm::scale(model, glm::vec3(0.02));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void drawScoreLogo(Shader shader, int i, float aspect) {
		shader.use();
		shader.setVec3f("viewPos", glm::vec3(1, 1, 1));
		shader.setFloat("aalpha", 1);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.9, 0.9, 0));
		model = glm::scale(model, glm::vec3(0.1, 0.1*aspect, 1));
		model = glm::translate(model, glm::vec3(i,0,0));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}



private:
	int angle = 0;
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
	}
	float vertices[30] = {
		// positions          // texture Coords 
		-0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.0f,  1.0f,  1.0f,

		-0.5f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.0f,  1.0f,  0.0f
	};
};

