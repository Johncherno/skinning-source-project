#pragma once
#include<vector>//生成三角形内存空间
#include<GL/glew.h>
#include<GLFW/glfw3.h>//要调用的显卡程序数据库
#include<glm.hpp>
class Drawcube
{
public:
	void PushVerticesToGPU();
	void draw();
private:
	//立方体坐标内存 
	void PushourcubeVertice();//放进去立方体坐标
	std::vector<glm::vec3>cubeMemoryVertices;
	GLuint VAO; GLuint uu;
};

