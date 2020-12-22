#pragma once
#include <glad/glad.h>
class TexCubeMap
{
public:
	// holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	unsigned int ID;
	// texture image dimensions
	unsigned int Width, Height; // width and height of loaded image in pixels
	// texture Format
	unsigned int Internal_Format; // format of texture object
	unsigned int Image_Format; // format of loaded image
	// texture configuration
	unsigned int Wrap_S; // wrapping mode on S axis
	unsigned int Wrap_T; // wrapping mode on T axis
	unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
	unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
	// constructor (sets default texture modes)
	TexCubeMap();
	// generates texture from image data
	void Generate(unsigned int width, unsigned int height, unsigned char* data);
	// binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};

