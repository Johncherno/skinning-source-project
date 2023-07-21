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
	glm::vec3 normal;// λ��  ��������   ����
    glm::vec3 Tangent;//���߿ռ�
	float BoneTranSformMatrixIndex[Max_Bone_influences];//��¼ÿ����������
	float m_Weights[Max_Bone_influences];//ÿ��������Ӧ��Ȩ��
};
class Mesh
{
  public:
	  Mesh(std::vector<SKeleTonMeshVertex>vertices, std::vector<unsigned int>indices);
	  std::vector<SKeleTonMeshVertex>vertices;//�洢��������
	  std::vector<unsigned int>indices;//�洢������������
	  void Draw();
	  
private:
	GLuint VAO, VBO, EBO;
	void setupmesh();
	
	
};

