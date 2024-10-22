#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


#include <string>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "GameMap.h"
#include "Texture.h"
#include "shader.h"

class ResourceManager
{
public:
	static Shader LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile);
	static Texture LoadTexture2D(const char * file, bool isAlpha,int Filter = GL_LINEAR, int Wrap = GL_REPEAT);
	static Texture LoadTextureCube(const char * file,int Filter = GL_LINEAR, int Wrap = GL_CLAMP_TO_EDGE);
	static GameMap LoadGameMap(const char * file);

private:
	ResourceManager() { }
	static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
};
#endif