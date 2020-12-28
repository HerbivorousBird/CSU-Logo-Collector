#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


#include <map>
#include <string>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "GameMap.h"
#include "Texture.h"
#include "shader.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader>  Shaders;
	static std::map<std::string, Texture> Textures;
	static std::map<std::string, GameMap> GameMaps;
	static Shader LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
	static Shader GetShader(std::string name);
	static Texture LoadTexture2D(const char * file, bool isAlpha, std::string name, int Filter = GL_LINEAR, int Wrap = GL_REPEAT);
	static Texture LoadTextureCube(const char * file, std::string name, int Filter = GL_LINEAR, int Wrap = GL_CLAMP_TO_EDGE);
	static Texture GetTexture(std::string name);
	static GameMap LoadGameMap(const char * file, std::string name);
	static GameMap GetGameMap(std::string name);

	static void      Clear();
private:
	ResourceManager() { }
	static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
};
#endif