#pragma once
#include "glm/glm.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

class GameMap{
public:

	int mapx;
	int mapz;
	int type;
	int sum;
	int* mapArr;
	glm::ivec3 roalPos;
	glm::ivec3 targetPos;

	void initFromTxt(char filePath[])
	{
		sum = 0;
		char temp[1000];
		ifstream file(filePath);
		if (!file)
		{
			cout << filePath << "未找到，请将地图置于特定目录下\n";
			return;
		}
		file.getline(temp, 1000, '\n'); //跳过文件第一行注释
		file >> mapz >> mapx >> type;
		file >> roalPos.x >> roalPos.z;
		file >> targetPos.x >> targetPos.z;
		mapArr = new int[mapx * mapz];
		for (int z = 0; z < mapz; z++) {
			for (int x = 0; x < mapx; x++) {
				file >> mapArr[z*mapx + x];
				sum += mapArr[z*mapx + x];
			}
		}
		roalPos.y = mapArr[roalPos.z * mapx + roalPos.x] + 2;
		targetPos.y = mapArr[targetPos.z * mapx + targetPos.x];
		file.close();
	}

	glm::ivec3 randTarget() {
		srand(unsigned(time(NULL)));
		int n = rand() % sum;
		return glm::ivec3(n%mapx, mapArr[n], n / mapx);
	}

	int * getMapPosArr()
	{
		int * result = new int[sum];
		int i = 0;
		for (int z = 0; z < mapz; z++) {
			for (int x = 0; x < mapx; x++) {
				result[i++] = x;
				result[i++] = mapArr[z*mapx + x];
				result[i++] = z;
			}
		}
		return result;
	}

private:
	
};