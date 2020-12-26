#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <glm/glm.hpp>
#include <iostream>
using namespace std;



#define roleHeight 1.6f           //玩家视点到脚的高度

#define GravityAcceler -10

#define JumpInitialSpeed 5.0f    //起跳初速度

#define XL 	0x0001
#define XR 	0x0010
#define ZB 	0x0100
#define ZT 	0x1000


struct dot {
	float x;
	float y;
	dot(float _x, float _y) :x(_x), y(_y) { }
};

class PhysicsEngine {
public:
	PhysicsEngine();
	~PhysicsEngine();

	void initBoundary(int* map, int mapx, int mapz);
	void jumpAndUpdateVelocity();    //跳跃时调用

	//每帧绘制的时候更新摄像机垂直方向移动
	void updateCameraVertMovement(glm::vec3 lastPos, glm::vec3 & newPos,float dt);

private:

	glm::ivec3 getLocation(glm::vec3 pos);
	int encode(glm::vec3 pos);
	void collisionTest(glm::vec3 lastPos, glm::vec3 & newPos);
	void testXZCollision(glm::vec3 & pos, glm::ivec3 loc);
	void updataNeighbour(glm::ivec3 loc);
	float vy;        //垂直方向速度
	bool isJumping;  //是否在跳跃
	int* map;        //地图数组
	int mapx, mapz;  //地图尺寸
	bool accessibleNei[10];  //邻域是否可进入
};

#endif // !PHYSICSENGINE_H



