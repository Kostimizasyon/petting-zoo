#pragma once
#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>

class Texture
{
public:
	GLuint ID;

	Texture(int widthImg, int heightImg, unsigned char* bytes);

	void Bind();
	void Unbind();
	void Delete();
};

#endif
