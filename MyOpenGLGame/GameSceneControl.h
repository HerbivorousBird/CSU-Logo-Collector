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
#include "Square.h"
#include "ParticleSystem.h"
#include <iostream>

enum Scene {
	MAINMENU,
	HELP_C,
	HELP_M,
	COLLECT,
	MAZE,
	WIN
};

class GameSceneControl
{
public:
	Scene currentScene;

	Cube GameSceneControl::cube;
	CSUlogo GameSceneControl::logo;
	Square square;
	GameMap GameSceneControl::gmap;
	Shader GameSceneControl::cubeShader;
	Shader GameSceneControl::logoShader;
	Shader GameSceneControl::skyboxShader;
	Shader GameSceneControl::fullPicShader;
	Shader GameSceneControl::hawkeyeShader;

	Texture GameSceneControl::cubeTex1;
	Texture GameSceneControl::cubeTex2;
	Texture GameSceneControl::logoTex;
	Texture GameSceneControl::skyboxTex;
	Texture GameSceneControl::mainMenuPic;
	Texture GameSceneControl::helpPic_m;
	Texture GameSceneControl::helpPic_c;
	Texture GameSceneControl::winPic;
	PhysicsEngine GameSceneControl::pEngine;
	Camera GameSceneControl::camera;
	ParticleSystem particle;

	int collectScore;
	bool isWin = false;

	glm::vec3 lastPos;
	glm::mat4 projection;
	glm::mat4 view;

	void initResourse() {
		cubeShader = ResourceManager::LoadShader("resources/shader/cube.vs", "resources/shader/cube.fs", nullptr);
		logoShader = ResourceManager::LoadShader("resources/shader/logo.vs", "resources/shader/logo.fs", nullptr);
		skyboxShader = ResourceManager::LoadShader("resources/shader/skybox.vs", "resources/shader/skybox.fs", nullptr);
		fullPicShader = ResourceManager::LoadShader("resources/shader/fullPic.vs", "resources/shader/fullPic.fs", nullptr);
		hawkeyeShader = ResourceManager::LoadShader("resources/shader/hawkeye.vs", "resources/shader/hawkeye.fs", nullptr); 

		mainMenuPic = ResourceManager::LoadTexture2D("resources/textures/mainMenuPic.png", true);
		helpPic_m = ResourceManager::LoadTexture2D("resources/textures/helpPic_m.png", true);
		helpPic_c = ResourceManager::LoadTexture2D("resources/textures/helpPic_c.png", true);
		winPic = ResourceManager::LoadTexture2D("resources/textures/winPic.png", true);

		logoTex = ResourceManager::LoadTexture2D("resources/textures/logo.png", true);
		skyboxTex = ResourceManager::LoadTextureCube("resources/textures/skybox/");
		cubeTex1 = ResourceManager::LoadTextureCube("resources/textures/grassland/", GL_NEAREST);
		cubeTex2 = ResourceManager::LoadTextureCube("resources/textures/stone/", GL_NEAREST);

		collectScore = 0;
		currentScene = MAINMENU;
	}

	void initMap() {
		if (currentScene == COLLECT) {
			gmap = ResourceManager::LoadGameMap("resources/maps/CollectMap.txt");
		}
		else if (currentScene == MAZE) {
			gmap = ResourceManager::LoadGameMap("resources/maps/MazeMap.txt");
		}
		else {
			gmap = ResourceManager::LoadGameMap("resources/maps/CollectMap.txt");
		}
		pEngine.initBoundary(gmap);
		camera.Position = gmap.roalLoc;
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

	void DrawFullScreen() {
		fullPicShader.use();
		glBindVertexArray(square.VAO);
		glActiveTexture(GL_TEXTURE0);
		switch (currentScene)
		{
		case MAINMENU:
			mainMenuPic.Bind();
			break;
		case HELP_C:
			helpPic_c.Bind();
			break;
		case HELP_M:
			helpPic_m.Bind();
			break;
		case WIN:
			winPic.Bind();
			break;
		default:
			break;
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void DrawHawkeye(float aspect) {
		glDepthFunc(GL_ALWAYS);
		if (currentScene != MAZE)
			return;
		hawkeyeShader.use();
		glBindVertexArray(square.VAO);
		glm::vec4 color;
		auto po = glm::ivec3(0,0,0);
		for (int z = 0; z < gmap.mapz; z++)
		{
			for (int x = 0; x < gmap.mapx; x++) {
				int y = gmap.getY(x, z);
				if (y == 1)
					color = glm::vec4(0.3, 0.8, 0.3, 0.8);
				if (y == 3)
					color = glm::vec4(0.1, 0.1, 0.1, 1);
				po.x = x;
				po.y = -z;
				square.drawHawkeyeSquare(hawkeyeShader,po,color ,aspect);
			}
		}
		color = glm::vec4(0, 0, 1, 1);
		po.x = pEngine.newLoc.x;
		po.y = -pEngine.newLoc.z;
		square.drawHawkeyeSquare(hawkeyeShader,po, color, aspect);

		color = glm::vec4(1, 0, 1, 1);
		po.x = gmap.targetLoc.x;
		po.y = -gmap.targetLoc.z;
		square.drawHawkeyeSquare(hawkeyeShader, po, color, aspect);
		glDepthFunc(GL_LESS);
	}

	void DrawCollectScore(float aspect) {
		glDepthFunc(GL_ALWAYS);
		if (currentScene != COLLECT)
			return;
		logoShader.use();
		logoShader.setMat4("projection", glm::mat4(1.0f));
		logoShader.setMat4("view", glm::mat4(1.0f));
		glBindVertexArray(logo.VAO);
		glActiveTexture(GL_TEXTURE0);
		logoTex.Bind();
		for (int i = 0; i < collectScore; i++)
		{
			logo.drawScoreLogo(logoShader, i, aspect);
		}
		glDepthFunc(GL_LESS);
	}

	void DrawMap() {
		cubeShader.use();
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);
		glBindVertexArray(cube.VAO);
		glActiveTexture(GL_TEXTURE0);
		if (currentScene == MAZE)
			cubeTex2.Bind();
		else
			cubeTex1.Bind();
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
		logo.drawLogo(gmap.targetLoc, logoShader,camera.Position);
	}

	void DrawParticle() {
		if (particle.particles.size() == 0) {
			return;
		}
		particle.updataStatus(0.001);
		logoShader.use();
		logoShader.setMat4("projection", projection);
		logoShader.setMat4("view", view);
		glBindVertexArray(logo.VAO);
		glActiveTexture(GL_TEXTURE0);
		logoTex.Bind();
		for (int i = 0; i < particle.particles.size(); i++)
		{
			Particle& p = particle.particles[i];
			float alpha = 1 - p.age / p.life;
			logo.drawParticleLogo(p.position,logoShader, alpha, p.angle);
		}
	}

	void DrawSkybox() {
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		glBindVertexArray(cube.VAO);
		glActiveTexture(GL_TEXTURE0);
		skyboxTex.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}
};


#endif
