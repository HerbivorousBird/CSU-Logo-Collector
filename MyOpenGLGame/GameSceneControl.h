#ifndef GAMESCENECONTROL_H
#define GAMESCENECONTROL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "Cube.h"
#include "CSUlogo.h"
#include "PhysicsEngine.h"
#include "resource_manager.h"
#include "GameMap.h"
#include "FullScreenPic.h"
#include <iostream>

enum Scene {
	MAINMENU,
	HELP,
	MAZE,
	COLLECT,
	WIN
};

class GameSceneControl
{
public:
	Scene currentScene;
	Cube GameSceneControl::cube;
	CSUlogo GameSceneControl::logo;
	FullScreenPic fullScene;
	GameMap GameSceneControl::gmap;
	Shader GameSceneControl::cubeShader;
	Shader GameSceneControl::logoShader;
	Shader GameSceneControl::skyboxShader;
	Shader GameSceneControl::fullPicShader;
	Texture GameSceneControl::cubeTex1;
	Texture GameSceneControl::cubeTex2;
	Texture GameSceneControl::logoTex;
	Texture GameSceneControl::skyboxTex;
	Texture GameSceneControl::mainMenuPic;
	PhysicsEngine GameSceneControl::pEngine;
	Camera GameSceneControl::camera;

	glm::vec3 lastPos;
	glm::mat4 projection;
	glm::mat4 view;

	void initResourse() {
		cubeShader = ResourceManager::LoadShader("resources/shader/cube.vs", "resources/shader/cube.fs", nullptr, "cube");
		logoShader = ResourceManager::LoadShader("resources/shader/logo.vs", "resources/shader/logo.fs", nullptr, "logo");
		skyboxShader = ResourceManager::LoadShader("resources/shader/skybox.vs", "resources/shader/skybox.fs", nullptr, "skyboxShader");
		fullPicShader = ResourceManager::LoadShader("resources/shader/fullPic.vs", "resources/shader/fullPic.fs", nullptr, "z");

		mainMenuPic = ResourceManager::LoadTexture2D("resources/textures/透明校徽2.png", true, "logoTex");
		ResourceManager::LoadTexture2D("resources/textures/brickwall.jpg", false, "wallTex");
		logoTex = ResourceManager::LoadTexture2D("resources/textures/透明校徽2.png", true, "logoTex");
		skyboxTex = ResourceManager::LoadTextureCube("resources/textures/skybox/", "skyboxTex");
		cubeTex1 = ResourceManager::LoadTextureCube("resources/textures/grassland/", "grasslandTex", GL_NEAREST);
	}

	void setMap(const char* file) {
		GameSceneControl::gmap = ResourceManager::LoadGameMap(file,"a");
		pEngine.initBoundary(gmap);
		camera.Position = gmap.roalPos;
		lastPos = camera.Position;
	}

	void bindTex2Sha() {
		cubeShader.use();
		cubeShader.setInt("texture1", 0);
		cubeShader.setInt("texture2", 1);
		skyboxShader.use();
		skyboxShader.setInt("skybox", 0);
		logoShader.use();
		logoShader.setInt("texture1", 0);
		fullPicShader.use();
		fullPicShader.setInt("texture1", 0);
	}


	void updataPara(float aspect,float deltaTime) {
		pEngine.updateCamera(lastPos, camera.Position, deltaTime);
		lastPos = camera.Position;
		logo.updateAngle();
		projection = glm::perspective(glm::radians(camera.Zoom), aspect, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
	}



	void DrawMainMenu() {
		fullPicShader.use();
		glBindVertexArray(fullScene.VAO);
		glActiveTexture(GL_TEXTURE0);
		mainMenuPic.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void DrawMap() {
		cubeShader.use();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);
		glBindVertexArray(cube.VAO);
		glActiveTexture(GL_TEXTURE0);
		cubeTex1.Bind();
		// render boxes
		for (unsigned int z = 0; z < gmap.mapz; z++)
		{
			for (unsigned int x = 0; x < gmap.mapx; x++) {
				int mapY = gmap.getY(x,z);
				for (unsigned int y = 0; y < mapY; y++) {
					auto po = glm::ivec3(x,y,z);
					cube.drawCube(po, cubeShader);
				}
			}
		}
	}

	void DrawLogo() {
		logoShader.use();
		logoShader.setMat4("projection", projection);
		logoShader.setMat4("view", view);
		glBindVertexArray(logo.VAO);
		glActiveTexture(GL_TEXTURE0);
		logoTex.Bind();
		logo.drawLogo(gmap.roalPos + glm::ivec3(0, 0, 1), logoShader);
	}

	void DrawSkybox() {
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(cube.VAO);
		glActiveTexture(GL_TEXTURE0);
		skyboxTex.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
	}
};


#endif
