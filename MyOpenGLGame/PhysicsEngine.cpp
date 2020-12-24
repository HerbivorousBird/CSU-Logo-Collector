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

// 粗略获取脚所在方格
glm::ivec3 PhysicsEngine::getLocation(glm::vec3 pos) { 
	glm::ivec3 location;
	location.x = (int)(pos.x + 0.5f);
	location.y = (int)(pos.y - roleHeight + 0.6f);
	location.z = (int)(pos.z + 0.5f);
	return location;
}

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


void PhysicsEngine::collisionTest(glm::vec3 &lastPos, glm::vec3 & newPos) {
	//newPos.x = newPos.x < 0 ? 0 : newPos.x;
	//newPos.x = newPos.x > mapx - 1 ? mapx - 1 : newPos.x;
	//newPos.z = newPos.z < 0 ? 0 : newPos.z;
	//newPos.z = newPos.z > mapz - 1 ? mapz - 1 : newPos.z;
	auto newLoc = getLocation(newPos);
	float y = map[newLoc.z * mapx + newLoc.x] + 1.1f; //该方格最低视点高度
	auto lastLoc = getLocation(lastPos);
	glm::ivec3 dLoc = newLoc - lastLoc;
	// Y方向碰撞检测
	if (newPos.y < y) {
		if (vy < 0.0f) {
			newPos.y = lastPos.y;
			newLoc.y = lastLoc.y;
			vy = 0.0f;
			isJumping = false;
		}
	}
	// XZ平面碰撞检测
	//if (newPos.y < y) {
	//	if (dLoc.x != 0) {
	//		newPos.x = lastPos.x;
	//		cout << "x " << map[lastLoc.z * mapx + lastLoc.x] << map[newLoc.z * mapx + newLoc.x] << endl;
	//	}
	//	if (dLoc.z != 0) {
	//		newPos.z = lastPos.z;
	//		cout << "z " << map[lastLoc.z * mapx + lastLoc.x] << map[newLoc.z * mapx + newLoc.x] << endl;
	//		cout << "y " << y << " ny " << newPos.y << endl;
	//	}
	//}
	//// 防止斜对角穿墙
	//if (dLoc.x != 0 && dLoc.z != 0) {
	//	newPos.x = lastPos.x;
	//	newPos.z = lastPos.z;
	//}
	int cases = isCollision(newPos, newLoc);
	//switch (cases)
	//{
	//case 0:
	//	break;
	//case 1:
	//	newPos.z = lastPos.z;
	//	cout << "x " << map[lastLoc.z * mapx + lastLoc.x] << map[newLoc.z * mapx + newLoc.x] << endl;
	//case 2:
	//	newPos.x = lastPos.x;
	//	cout << "x " << map[lastLoc.z * mapx + lastLoc.x] << map[newLoc.z * mapx + newLoc.x] << endl;
	//default:
	//	break;
	//}
	//if (isCollision(newPos, newLoc)) {
	//	newPos.z = lastPos.z;
	//	newPos.x = lastPos.x;
	//}

}

int PhysicsEngine::isCollision(glm::vec3 &pos, glm::ivec3 loc) {
	int code = encode(pos);
	if (code == 0x0000) {
		return 0;
	}
	if (XL & code && isLowerThenNeighborhood(loc, XL)) {
		pos.x = loc.x - 0.3;
	}
	if (XR & code && isLowerThenNeighborhood(loc, XR)) {
		pos.x = loc.x + 0.3;
	}
	if (ZB & code && isLowerThenNeighborhood(loc, ZB)) {
		pos.z = loc.z - 0.3;
	}
	if (ZT & code && isLowerThenNeighborhood(loc, ZT)) {
		pos.z = loc.z + 0.3;
	}
	if (code == 0x1001&& isLowerThenNeighborhood(loc, 0x1001)) {
		pos.x = loc.x - 0.3;
		pos.z = loc.z + 0.3;
	}
	if (code == 0x1010 && isLowerThenNeighborhood(loc, 0x1010)) {
		pos.x = loc.x + 0.3;
		pos.z = loc.z + 0.3;
	}
	if (code == 0x0101 && isLowerThenNeighborhood(loc, 0x0101)) {
		pos.x = loc.x - 0.3;
		pos.z = loc.z - 0.3;
	}
	if (code == 0x0110 && isLowerThenNeighborhood(loc, 0x0110)) {
		pos.x = loc.x + 0.3;
		pos.z = loc.z - 0.3;
	}
	return 0;
}

bool PhysicsEngine::isLowerThenNeighborhood(glm::ivec3 loc, int neiborCode) {
	int* currentP = &map[loc.z * mapx + loc.x];
	int currentY = *currentP;
	switch (neiborCode) {
	case 0x1001: //左上角(LT)
		if (loc.x == 0 || loc.z == mapz-1)
			return true;
		if (*(currentP + mapx - 1) > loc.y)
			return true;
		break;
	case 0x1000: //上边
		if (loc.z == mapz-1)
			return true;
		if (*(currentP + mapx) > loc.y)
			return true;
		break;
	case 0x1010: //右上角(RT)
		if (loc.x == mapx - 1 || loc.z == mapz-1)
			return true;
		if (*(currentP + mapx + 1) > loc.y)
			return true;
		break;
	case 0x0001: //左边
		if (loc.x == 0)
			return true;
		if (*(currentP - 1) > loc.y)
			return true;
		break;
	case 0x0010: //右边
		if (loc.x == mapx - 1)
			return true;
		if (*(currentP + 1) > loc.y)
			return true;
		break;
	case 0x0101: //左下角(LB)
		if (loc.x == 0 || loc.z == 0)
			return true;
		if (*(currentP - mapx - 1) > loc.y)
			return true;
		break;
	case 0x0100: //下边
		if (loc.z == 0)
			return true;
		if (*(currentP - mapx) > loc.y)
			return true;
		break;
	case 0x0110: //右下角(RB)
		if (loc.x == mapx - 1 || loc.z == 0)
			return true;
		if (*(currentP - mapx + 1) > loc.y)
			return true;
		break;
	}
	return false;
}


void PhysicsEngine::jumpAndUpdateVelocity() {
	if (!isJumping)
		vy += JumpInitialSpeed;
	isJumping = true;
}


void PhysicsEngine::updateCameraVertMovement(glm::vec3 lastPos, glm::vec3 & newPos, float dt) {
	newPos.y += (vy*dt + GravityAcceler * dt * dt / 2);
	vy += GravityAcceler * dt;
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
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2>0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4>0)))
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
		if (targetPos[0] < cameraPos[0]) {
			printf("3\n");
			targetPos[0] = x2;
			cameraPos[0] += (targetPos[0] - tarX);
		}
		else if (targetPos[0] > cameraPos[0]) {
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
