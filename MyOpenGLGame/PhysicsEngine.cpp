#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {
	vy = 0.0f;
	isJumping = false;
}

void PhysicsEngine::initBoundary(GameMap &gmap) {
	this->map = gmap.mapArr;
	this->mapx = gmap.mapx;
	this->mapz = gmap.mapz;
	this->gmap = &gmap;
}

// 粗略获取脚所在方格
glm::ivec3 PhysicsEngine::getLocation(glm::vec3 pos) { 
	glm::ivec3 location;
	location.x = (int)(pos.x + 0.5f);
	location.y = (int)(pos.y - roleHeight + 0.6f); //稍高一些，防止跨方格时“抖动感”
	location.z = (int)(pos.z + 0.5f);
	return location;
}

// 八区域编码
int PhysicsEngine::encode(glm::vec3 pos)
{
	auto loc = getLocation(pos);
	int c = 0x0000;
	if (pos.x < loc.x - 0.3) c |= XL;
	if (pos.x > loc.x + 0.3) c |= XR;
	if (pos.z < loc.z - 0.3) c |= ZB;
	if (pos.z > loc.z + 0.3) c |= ZT;
	return c;
}

PhysicsEngine::~PhysicsEngine() {

}


void PhysicsEngine::collisionTest(glm::vec3 lastPos, glm::vec3 & newPos) {
	newLoc = getLocation(newPos);
	float y = map[newLoc.z * mapx + newLoc.x] -0.5f+roleHeight; //该方格最低视点高度
	lastLoc = getLocation(lastPos);
	//glm::ivec3 dLoc = newLoc - lastLoc;
	// Y方向碰撞检测
	if (newPos.y < y) {
		if (vy < 0.0f) {
			newPos.y = lastPos.y;
			newLoc.y = lastLoc.y;
			vy = 0.0f;
			isJumping = false;
		}
	}
	// XZ 方向碰撞检测
	if(lastLoc != newLoc)
		updataNeighbour(newLoc);
	testXZCollision(newPos, newLoc);
}

void PhysicsEngine::testXZCollision(glm::vec3 &pos, glm::ivec3 loc) {
	code = encode(pos);
	if (code == 0x0000) {
		return;
	}
	if (XL & code && !accessibleNei[4]) {
		pos.x = loc.x - 0.3;
	}
	if (XR & code && !accessibleNei[6]) {
		pos.x = loc.x + 0.3;
	}
	if (ZB & code && !accessibleNei[8]) {
		pos.z = loc.z - 0.3;
	}
	if (ZT & code && !accessibleNei[2]) {
		pos.z = loc.z + 0.3;
	}
	// 进入四个角
	if (code == 0x1001 && !accessibleNei[1]) {
		if (accessibleNei[2] || accessibleNei[4])
			return;
		if (accessibleNei[2] && accessibleNei[4]) {
			if (abs(pos.z - loc.z) < abs(pos.x - loc.x))
				pos.z = loc.z + 0.3;
			else
				pos.x = loc.x - 0.3;
			return;
		}
		pos.x = loc.x - 0.3;
		pos.z = loc.z + 0.3;
	}
	if (code == 0x1010 && !accessibleNei[3]) {
		if (accessibleNei[6] || accessibleNei[2])
			return;
		if (accessibleNei[6] && accessibleNei[2]) {
			if (abs(pos.z - loc.z) < abs(pos.x - loc.x))
				pos.z = loc.z + 0.3;
			else
				pos.x = loc.x + 0.3;
			return;
		}
		pos.x = loc.x + 0.3;
		pos.z = loc.z + 0.3;
	}
	if (code == 0x0101 && !accessibleNei[7]) {
		if (accessibleNei[4] || accessibleNei[8])
			return;
		if (accessibleNei[4] && accessibleNei[8]) {
			if (abs(pos.z - loc.z) < abs(pos.x - loc.x))
				pos.z = loc.z - 0.3;
			else
				pos.x = loc.x - 0.3;
			return;
		}
		pos.x = loc.x - 0.3;
		pos.z = loc.z - 0.3;
	}
	if (code == 0x0110 && !accessibleNei[9]) {
		if (accessibleNei[6] || accessibleNei[8])
			return;
		if (accessibleNei[6] && accessibleNei[8]) {
			if (abs(pos.z - loc.z) < abs(pos.x - loc.x))
				pos.z = loc.z - 0.3;
			else
				pos.x = loc.x + 0.3;
			return;
		}
		pos.x = loc.x + 0.3;
		pos.z = loc.z - 0.3;
	}
}

void PhysicsEngine::updataNeighbour(glm::ivec3 loc)
{
	int* currentP = &map[loc.z * mapx + loc.x];
	for (int i = 1; i < 10; i++) {
		bool result = true;
		switch (i) {
		case 1: //左上角(LT)
			if (loc.x == 0 || loc.z == mapz - 1)
				result = false;
			if (*(currentP + mapx - 1) > loc.y)
				result = false;
			break;
		case 2: //上边
			if (loc.z == mapz - 1)
				result = false;
			if (*(currentP + mapx) > loc.y)
				result = false;
			break;
		case 3: //右上角(RT)
			if (loc.x == mapx - 1 || loc.z == mapz - 1)
				result = false;
			if (*(currentP + mapx + 1) > loc.y)
				result = false;
			break;
		case 4: //左边
			if (loc.x == 0)
				result = false;
			if (*(currentP - 1) > loc.y)
				result = false;
			break;
		case 6: //右边
			if (loc.x == mapx - 1)
				result = false;
			if (*(currentP + 1) > loc.y)
				result = false;
			break;
		case 7: //左下角(LB)
			if (loc.x == 0 || loc.z == 0)
				result = false;
			if (*(currentP - mapx - 1) > loc.y)
				result = false;
			break;
		case 8: //下边
			if (loc.z == 0)
				result = false;
			if (*(currentP - mapx) > loc.y)
				result = false;
			break;
		case 9: //右下角(RB)
			if (loc.x == mapx - 1 || loc.z == 0)
				result = false;
			if (*(currentP - mapx + 1) > loc.y)
				result = false;
			break;
		default:
			result = true;
		}
		accessibleNei[i] = result;
	}
}


void PhysicsEngine::jump() {
	if (!isJumping)
		vy += JumpInitialSpeed;
	isJumping = true;
}


void PhysicsEngine::updateCamera(glm::vec3 lastPos, glm::vec3 & newPos, float dt) {
	newPos.y += (vy*dt + GravityAcceler * dt * dt / 2);
	vy += GravityAcceler * dt;
	collisionTest(lastPos, newPos);
}

bool PhysicsEngine::isCollectable() {
	return (newLoc+glm::ivec3(0,1,0) == gmap->targetLoc && code == 0x0000);
}