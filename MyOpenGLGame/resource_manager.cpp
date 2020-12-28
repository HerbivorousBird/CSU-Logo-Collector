#include "resource_manager.h"

std::map<std::string, Texture> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, GameMap> ResourceManager::GameMaps;

Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture ResourceManager::LoadTexture2D(const char *file,bool isAlpha, std::string name, int Filter, int Wrap)
{
	Texture texture(GL_TEXTURE_2D);
	if(isAlpha)
		texture.initPara(GL_RGBA, Filter, Wrap);
	else
		texture.initPara(GL_RGB, Filter, Wrap);
	texture.Generate(file);
	Textures[name] = texture;
    return Textures[name];
}

Texture ResourceManager::LoadTextureCube(const char *file, std::string name, int Filter, int Wrap)
{
	Texture texture(GL_TEXTURE_CUBE_MAP);
	texture.initPara(GL_RGB, Filter, Wrap);
	texture.Generate(file);
	Textures[name] = texture;
	return Textures[name];
}

Texture ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

GameMap ResourceManager::LoadGameMap(const char * file, std::string name)
{
	GameMap gmap;
	gmap.initFromTxt(file);
	GameMaps[name] = gmap;
	return GameMaps[name];
}

GameMap ResourceManager::GetGameMap(std::string name)
{
	return GameMaps[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}
