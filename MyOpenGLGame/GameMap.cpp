#include "GameMap.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;


GameMap::GameMap()
{
}

void GameMap::initFromTxt(char filePath[])
{
	sum = 0;
	char temp[1000];
	ifstream file(filePath);
	if (!file)
	{
		cout << filePath << "未找到，请将其置于特定目录下\n";
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
			sum+= mapArr[z*mapx + x];
		}
	}
	roalPos.y = mapArr[roalPos.z * mapx + roalPos.x]+2;
	targetPos.y = mapArr[targetPos.z * mapx + targetPos.x];
	file.close();
}



int * GameMap::getMapPosArr()
{
	int * result = new int[sum];
	int i = 0;
	for (int z = 0; z < mapz; z++) {
		for (int x = 0; x < mapx; x++) {
			result[i++] = x;
			result[i++] = z;
			result[i++] = mapArr[z*mapx + x];
		}
	}
	return result;
}


