#pragma once
#include<vector>//�����������ڴ�ռ�
#include<GL/glew.h>
#include<GLFW/glfw3.h>//Ҫ���õ��Կ��������ݿ�
#include<glm.hpp>
class Drawcube
{
public:
	void PushVerticesToGPU();
	void draw();
private:
	//�����������ڴ� 
	void PushourcubeVertice();//�Ž�ȥ����������
	std::vector<glm::vec3>cubeMemoryVertices;
	GLuint VAO; GLuint uu;
};

