#pragma once
#include<glm.hpp>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
struct SkeletonVertex
{
	glm::vec3 Vertice;//����
	glm::vec2 Texcoord;//��������
	glm::vec3 Normal;//����
	glm::vec2 WEighting;//Ȩ��
};
class MD5mesh
{
public:
	void Bind_PerskeletonVertice_To_GPU();
	void Create_VAO_VBO_EBO_Buffer_Object();
	void Draw();
	int *indices;
	int num_vertices;//��������
	int num_indices;//��������
	SkeletonVertex* skeletonVert;
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};

