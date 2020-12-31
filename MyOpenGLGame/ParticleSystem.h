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
	float life;
	int angle;
};

class ParticleSystem
{
public:
	ParticleSystem() {}
	std::vector<Particle> particles;
	float age;
	~ParticleSystem() {}

	void ParticleSystem::init(int count, glm::vec3 basePos)
	{
		particles.clear();
		ptlCount = count;
		age = 0;
		srand(unsigned(time(0)));
		for (int i = 0; i < ptlCount; i++) {
			Particle tmp = {
				basePos,	//Position
				glm::vec3(((rand() % 20) - 10.0f) / 10,((rand() % 40)) / 10,((rand() % 20 - 10.0f)) / 10),//velocity
				1.0 + 0.1 * (rand() % 10),	//Life
				rand() % 180	//angle
			};
			particles.push_back(tmp);
		}
	}


	void ParticleSystem::updataStatus(float dt) {
		age+=dt;
		for (int i = 0; i < particles.size(); i++) {
			//����λ������
			particles[i].position += particles[i].veclocity * dt;
			particles[i].veclocity += gravity * dt;
			//������ת�Ƕ�
			particles[i].angle = particles[i].angle++ % 360;
			//�ж�����
			if (age > particles[i].life) {
				particles.erase(particles.begin() + i);
			}
		}
	}

private:
	int ptlCount;
	glm::vec3 gravity = glm::vec3(0,-10.0f,0);
};