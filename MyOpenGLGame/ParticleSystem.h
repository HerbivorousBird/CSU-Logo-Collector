#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <iostream>
#include <time.h>
#include <glm/gtx/string_cast.hpp>

struct Particle
{
	glm::vec3 position;
	glm::vec3 veclocity;
	float age;
	float life;
	int angle;
};

class ParticleSystem
{
public:
	ParticleSystem() {}
	std::vector<Particle> particles;
	~ParticleSystem() {}

	void ParticleSystem::init(int count, glm::vec3 basePos)
	{
		particles.clear();
		ptlCount = count;
		srand(unsigned(time(0)));
		for (int i = 0; i < ptlCount; i++) {
			Particle tmp = {
				basePos,	//Position
				glm::vec3(((rand() % 20) - 10.0f) / 10,((rand() % 40)) / 10,((rand() % 20 - 10.0f)) / 10),//velocity
				0.0f,	//age
				1.0 + 0.1 * (rand() % 10),	//Life
				rand() % 180	//angle
			};
			particles.push_back(tmp);
		}
	}


	void ParticleSystem::updataStatus(float dt) {
		for (int i = 0; i < particles.size(); i++) {
			//更新年龄并判断消亡
			particles[i].age += dt;
			if (particles[i].age > particles[i].life) {
				particles.erase(particles.begin() + i);
			}
			//更新位置坐标
			particles[i].position += particles[i].veclocity * dt;
			particles[i].veclocity += gravity * dt;
			//更新旋转角度
			particles[i].angle = particles[i].angle++ % 360;
		}
	}

private:
	int ptlCount;
	glm::vec3 gravity = glm::vec3(0,-10.0f,0);
};