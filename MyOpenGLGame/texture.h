#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include "stb_image.h"

class Texture
{
public:
	unsigned int ID;
	int texType;
	int Width, Height;
	int Image_Format;
	int Wrap_S;
	int Wrap_T;
	int Wrap_R;
	int Filter_Min;
	int Filter_Max;

	Texture(unsigned int type= GL_TEXTURE_2D)
	{
		glGenTextures(1, &this->ID);
		this->texType = type;
	};

	void initPara(int Image_Format,int Filter, int Wrap) {
		this->Image_Format = Image_Format;
		this->Wrap_S = Wrap;
		this->Wrap_T = Wrap;
		this->Wrap_T = Wrap;
		this->Filter_Max = Filter;
		this->Filter_Min = Filter;
	}

	void Generate(const char *file) {
		if (texType == GL_TEXTURE_2D) {
			GenerateTex2D(file);
		}
		else {
			GenerateTexCube(file);
		}
	}

	void GenerateTex2D(const char *file)
	{
		unsigned char* data = stbi_load(file, &Width, &Height, NULL, 0);
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexImage2D(GL_TEXTURE_2D, 0, Image_Format, Width, Height, 0, Image_Format, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);

		glBindTexture(GL_TEXTURE_2D, 0);
	};

	void GenerateTexCube(const char *filepath) {
		std::string fileName[6] = { "right","left","top","bottom","front","back" };
		glBindTexture(GL_TEXTURE_CUBE_MAP,ID);
		for (int i = 0; i < 6; i++) {
			auto file = filepath + fileName[i] + ".jpg";
			unsigned char* data = stbi_load(file.data(), &Width, &Height, NULL, 0);
			if (data)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else
				std::cout << "Cubemap texture failed to load at path: " << file << std::endl;
			stbi_image_free(data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, Filter_Min);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, Filter_Max);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, Wrap_S);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, Wrap_T);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, Wrap_R);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void Bind()
	{
		glBindTexture(texType, this->ID);
	};
};