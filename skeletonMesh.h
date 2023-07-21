#pragma once
 #include<glm.hpp>
#include<string>
#include<vector>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include "shaderProgram.h"
#define Max_Bone_influences 4
struct SKeleTonMeshVertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;// 位置  纹理坐标   法线
    glm::vec3 Tangent;//切线空间
	float BoneTranSformMatrixIndex[Max_Bone_influences];//记录每个骨骼索引
	float m_Weights[Max_Bone_influences];//每个骨骼对应的权重
};
class Mesh
{
  public:
	  Mesh(std::vector<SKeleTonMeshVertex>vertices, std::vector<unsigned int>indices);
	  std::vector<SKeleTonMeshVertex>vertices;//存储顶点数据
	  std::vector<unsigned int>indices;//存储顶点数据索引
	  void Draw();
	  
private:
	GLuint VAO, VBO, EBO;
	void setupmesh();
	
	
};

