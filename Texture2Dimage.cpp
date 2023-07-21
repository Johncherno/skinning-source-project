#include"Texture2Dimage.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
void texture2D::loadtexture(const string& filename)
{
	glGenTextures(1, &mtexture);
	glBindTexture(GL_TEXTURE_2D, mtexture);//Œ∆¿ÌªØ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_BORDER);

	int width, height, channels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	//std::cout << data << std::endl;
	
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);
}
void texture2D::bindtexture(unsigned int unit)
{
	
	
    glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(GL_TEXTURE_2D, mtexture);
	
	//std::cout << GL_TEXTURE0 + unit << std::endl;
}

void texture2D::unbindtexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}




