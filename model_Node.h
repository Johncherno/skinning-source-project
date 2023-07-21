#pragma once
#include<glm.hpp>
#include"gtc/matrix_transform.hpp"
#include<vector>
#include"mesh.h"
#include"Texture2Dimage.h"
#include"shaderProgram.h"
class modelNode
{
public:
	modelNode(mesh* Mesh, texture2D* Tex);
	modelNode();
	mesh* GetMesh() { return Mesh; }
	void Set_modelscale(const glm::mat4& modelscale);
	void Set_Transform(const glm::mat4& translation);
	glm::mat4 Getaccumulate_transform() { return accumulatetransform; }
	glm::mat4 GetNodetransform() { return transform; }
	glm::mat4 GetbindPosetransform() { return bindposetransform; }
	void Set_bindPosetransform(const glm::mat4&model) {
		this->bindposetransform = model;
	}
	void Addchild(modelNode* child);
	void update(float msec);
	std::vector<modelNode*>::const_iterator Getchildbegin() { return children.begin(); }//��������ӽڵ��������׵�ַ
	std::vector<modelNode*>::const_iterator Getchildend() { return children.end(); }//��������ӽڵ�����β��ַ
	std::vector<modelNode*>Getchildrenarray() { return children; }
	void Draw(shaderProgram* animationshader);
	void Set_Node_name(const char*Name) {
		name = Name;
	}
	const char* Get_Node_name() { return name; }
protected:
	glm::mat4 accumulatetransform;//�۳˾���
	glm::mat4 transform;//����ľ���
	glm::mat4 Modelscale ;//���ž���
	glm::mat4 bindposetransform;//�����ռ����
	modelNode* parent;//����ڵ��ӳ�ĸ��ڵ�
	mesh* Mesh;
	texture2D* tex;
	const char* name;
	std::vector<modelNode*>children;//�ӽڵ�����
};

