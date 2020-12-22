#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {
	vy = 0.0f;
    isJumping = false;
}

void PhysicsEngine::initBoundary(int * map, int mapx, int mapz) {
	this->map = map;
	this->mapx = mapx;
	this->mapz = mapz;
}

glm::ivec3 PhysicsEngine::getLocation(glm::vec3 pos) {
	glm::ivec3 location;
	location.x = (int)(pos.x + 0.5f);
	location.y = (int)(pos.y - roleHeight + 0.5f);
	location.z = (int)(pos.z + 0.5f);
	return location;
}

PhysicsEngine::~PhysicsEngine() {

}


void PhysicsEngine::collisionTest(glm::vec3 lastPos, glm::vec3 & newPos) {
	glm::ivec3 newLoc = getLocation(newPos);
	if (newLoc.x < 0 || newLoc.x >= mapx || newLoc.z < 0 || newLoc.z >= mapz) {
		newPos.x = lastPos.x;
		newPos.z = lastPos.z;
	}
	
	if (newLoc.y < map[newLoc.x * mapz + newLoc.z]-0.1) {
		newPos.y = lastPos.y;
		cout << newLoc.y << endl;
		vy = 0.0f;
		isJumping = false;
	}
}


void PhysicsEngine::jumpAndUpdateVelocity() {	
	if(!isJumping)
		vy += JumpInitialSpeed;
	isJumping = true;
}


void PhysicsEngine::updateCameraVertMovement(glm::vec3 lastPos, glm::vec3 & newPos,float dt) {
	newPos.y += (vy*dt + GravityAcceler * dt * dt / 2);
	vy += GravityAcceler * dt;
	//std::cout << vy<<endl;
	collisionTest(lastPos, newPos);
}

double Direction(dot pi, dot pj, dot pk) {
	return (pk.x - pi.x)*(pj.y - pi.y) - (pj.x - pi.x)*(pk.y - pi.y);
}

bool OnSegment(dot pi, dot pj, dot pk) {
	if ((min(pi.x, pj.x) <= pk.x) && (pk.x <= max(pi.x, pj.x))
		&& (min(pi.y, pj.y) <= pk.y) && (pk.y <= max(pi.y, pj.y)))
		return true;
	else return false;
}

//检测线段相交快速算法
bool SegmentIntersect(dot p1, dot p2, dot p3, dot p4) {
	int d1, d2, d3, d4;
	d1 = Direction(p3, p4, p1);
	d2 = Direction(p3, p4, p2);
	d3 = Direction(p1, p2, p3);
	d4 = Direction(p1, p2, p4);
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2>0)) && ((d3>0 && d4 < 0) || (d3 < 0 && d4>0)))
		return true;
	else if (d1 == 0 && OnSegment(p3, p4, p1))
		return true;
	else if (d2 == 0 && OnSegment(p3, p4, p2))
		return true;
	else if (d3 == 0 && OnSegment(p1, p2, p3))
		return true;
	else if (d4 == 0 && OnSegment(p1, p2, p4))
		return true;
	else
		return false;
}

void PhysicsEngine::inCollisionTestXZ(float x1, float z1, float x2, float z2, glm::vec3 & cameraPos, glm::vec3 & targetPos) {
	const float d = 2.0f;
	float tarX = targetPos[0], camX = cameraPos[0], tarZ = targetPos[2], camZ = cameraPos[2];
	float len = sqrt((camX - tarX)*(camX - tarX) + (camZ - tarZ)*(camZ - tarZ));

	dot d1(cameraPos[0], cameraPos[2]), d2(targetPos[0], targetPos[2]);
	dot d3(x1, z1), d4(x1, z2), d5(x2, z1), d6(x2, z2);

	if (SegmentIntersect(d1, d2, d4, d6)) {
		if (targetPos[2] < cameraPos[2]) {
			printf("1\n");

			//利用相似三角形原理计算，
			//仅改变z坐标
			targetPos[2] = z2;
			cameraPos[2] += (targetPos[2] - tarZ);
		}
		else if (targetPos[2] > cameraPos[2]) {
			printf("2\n");
			cameraPos[2] = z2;
			targetPos[2] += (cameraPos[2] - camZ);
		}
	}
	else if (SegmentIntersect(d1, d2, d5, d6)) {
		if (targetPos[0]<cameraPos[0]) {
			printf("3\n");
			targetPos[0] = x2;
			cameraPos[0] += (targetPos[0] - tarX);
		}
		else if (targetPos[0]>cameraPos[0]) {
			printf("4\n");
			cameraPos[0] = x2;
			targetPos[0] += (cameraPos[0] - camX);
		}
	}
	else if (SegmentIntersect(d1, d2, d3, d5)) {
		if (targetPos[2] > cameraPos[2]) {
			printf("5\n");
			targetPos[2] = z1;
			cameraPos[2] += (targetPos[2] - tarZ);
		}
		else if (targetPos[2] < cameraPos[2]) {
			printf("6\n");
			cameraPos[2] = z1;
			targetPos[2] += (cameraPos[2] - camZ);
		}
	}
	else if (SegmentIntersect(d1, d2, d3, d4)) {
		if (targetPos[0] > cameraPos[0]) {
			printf("7\n");
			targetPos[0] = x1;
			cameraPos[0] += (targetPos[0] - tarX);
		}
		else if (targetPos[0] < cameraPos[0]) {
			printf("8\n");
			cameraPos[0] = x1;
			targetPos[0] += (cameraPos[0] - camX);
		}
	}
}
