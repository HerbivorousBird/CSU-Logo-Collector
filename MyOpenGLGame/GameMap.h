#pragma once
#include "glm/glm.hpp"
class GameMap{
public:
	GameMap();
	void initFromTxt(char file[]);
	void initFromImg();
	int * getMapPosArr();
	int mapx;
	int mapz;
	int type;
	int sum;
	int* mapArr;
	glm::ivec3 roalPos;
	glm::ivec3 targetPos;
private:
	
};