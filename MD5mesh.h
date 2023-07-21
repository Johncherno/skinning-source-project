#pragma once
#include<glm.hpp>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
struct SkeletonVertex
{
	glm::vec3 Vertice;//顶点
	glm::vec2 Texcoord;//纹理坐标
	glm::vec3 Normal;//法线
	glm::vec2 WEighting;//权重
};
class MD5mesh
{
public:
	void Bind_PerskeletonVertice_To_GPU();
	void Create_VAO_VBO_EBO_Buffer_Object();
	void Draw();
	int *indices;
	int num_vertices;//顶点数量
	int num_indices;//索引数量
	SkeletonVertex* skeletonVert;
private:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};

