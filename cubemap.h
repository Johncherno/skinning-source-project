#ifndef CUBEMAP_H
#define CUBEMAP_H
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<string>
#include<vector>
#include"shaderProgram.h"

class cubemap
{
public:
	void loadtexture(
		std::string right="teximage//right.jpg",
		std::string left="teximage//left.jpg",
		std::string top="teximage//top.jpg",
		std::string botttom="teximage//bottom.jpg",
		std::string front="teximage//front.jpg",
		std::string back="teximage//back.jpg");//������պ�ͼƬ
	void init();
	void render(glm::vec3& camera,glm::vec3& camerafront);
private:
	unsigned int id;
	std::string dir;//Ŀ¼
	std::vector<std::string>faces;//�������
	bool hastexture;
	
	GLuint vao,vbo;
	
};									
#endif	
