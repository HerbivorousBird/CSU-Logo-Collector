#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
using namespace std;

#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) < (y) ? (y) : (x))

#define roleHeight 1.6f           //玩家视点到脚的高度

#define GravityAcceler -9.8f

#define MoveSpeed 0.15f           //玩家移动速度
#define BoundaryGap 1.0f          //碰撞间距
#define JumpInitialSpeed 5.0f    //起跳初速度

struct dot {
	float x;
	float y;
	dot(float _x, float _y) :x(_x), y(_y) { }
};

class PhysicsEngine {
public:
	PhysicsEngine();
	void initBoundary(int* map, int mapx, int mapz);
	glm::ivec3 getLocation(glm::vec3 pos);
	~PhysicsEngine();

	void collisionTest(glm::vec3 & lastPos, glm::vec3 & newPos);



	bool isJumping;
	void jumpAndUpdateVelocity();    //按下space跳跃时调用
	//每帧绘制的时候更新摄像机垂直方向移动
	void updateCameraVertMovement(glm::vec3 lastPos, glm::vec3 & newPos,float dt);

private:
	//空间内部边缘碰撞检测（不考虑高度，即XZ平面）
	void inCollisionTestXZ(float x1, float z1, float x2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos);

	glm::vec4 getNeighbourhood(glm::ivec3 loc);

	
	float vy;        //垂直方向速度

	int* map;
	int mapx, mapz;
	glm::vec4 outerBoundary;
};

#endif // !PHYSICSENGINE_H



