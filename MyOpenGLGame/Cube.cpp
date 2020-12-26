#include "Cube.h"

Cube::Cube(GameMap* map)
{
	this->map = map;
	setup_vertex();
}

Cube::~Cube()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Cube::setup_vertex() {
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

//void Cube::setup_texture() {
//	ourShader.use();
//	ourShader.setInt("texture1", 0);
//	ourShader.setInt("texture2", 1);
//}




void Cube::drawCube(glm::ivec3 position,Shader shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	shader.SetVector3f("translate",position);
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::drawSkybox(Shader shader) {

}
