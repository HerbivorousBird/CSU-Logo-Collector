#include "CSUlogo.h"

CSUlogo::CSUlogo()
{
	angle = 0;
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



void CSUlogo::updateAngle()
{
	angle = angle++ % 360;
}

void CSUlogo::drawLogo(glm::vec3 position, Shader shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(float(angle)), glm::vec3(0.0, 1, 0.0));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
