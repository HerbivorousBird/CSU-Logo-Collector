#ifndef GAMEMAP_H
#define GAMEMAP_H

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
	glm::ivec3 roalLoc;
	glm::ivec3 targetLoc;

	GameMap() {};

	void initFromTxt(const char filePath[])
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
		file >> roalLoc.x >> roalLoc.z;
		file >> targetLoc.x >> targetLoc.z;
		mapArr = new int[mapx * mapz];
		for (int z = 0; z < mapz; z++) {
			for (int x = 0; x < mapx; x++) {
				file >> mapArr[z*mapx + x];
				sum += mapArr[z*mapx + x];
			}
		}
		roalLoc.y = mapArr[roalLoc.z * mapx + roalLoc.x] + 2;
		targetLoc.y = mapArr[targetLoc.z * mapx + targetLoc.x]+1;
		file.close();
	}

	int getY(int x, int z) {
		return mapArr[z * mapx + x];
	}

	void randTarget() {
		srand(unsigned(time(NULL)));
		int n = rand() % (mapx*mapz);
		targetLoc=glm::ivec3(n % mapx, mapArr[n]+1, n / mapx);
		std::cout << n % mapx <<" "<< mapArr[n] + 2 << " " << n / mapx << endl;
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
};
#endif