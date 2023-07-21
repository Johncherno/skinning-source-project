#include "model_Node.h"
#include<iostream>
modelNode::modelNode( mesh* Mesh=nullptr, texture2D* Tex=nullptr)
	:accumulatetransform(glm::mat4(1.0f)),
	 Modelscale(glm::mat4(1.0f)),
	 transform(glm::mat4(1.0f))
{
	this->Mesh = Mesh;//����������������
	this->tex = Tex;//����������������
	this->parent = nullptr;
	
}
modelNode::modelNode()
	:accumulatetransform(glm::mat4(1.0f)), 
	 Modelscale(glm::mat4(1.0f)), 
	 transform(glm::mat4(1.0f))
{
	this->Mesh = nullptr;//����������������
	this->tex = nullptr;//����������������
	this->parent = nullptr;
	
}
void modelNode::Set_modelscale(const glm::mat4& modelscale)
{
	Modelscale = modelscale;
}
void modelNode::Set_Transform(const glm::mat4& translation) {
	transform = translation ;//�����Լ��Ľڵ�ı任����
}
void modelNode::Addchild(modelNode* child) {
	children.push_back(child);
	child->parent = this;
}
void modelNode::update(float msec)
{    
	//std::cout<<"we are excuting modelnode programing" << msec << std::endl;
	
	if (parent )//�㲻�Ǹ��ڵ�
	{
		accumulatetransform = parent->accumulatetransform * transform*Modelscale;//���Լ��ı任���������һ�����׽ڵ�ľ���
		//std::cout << name << std::endl;
	}
	else//��ô����Ǹ��ڵ�
	{
		accumulatetransform = transform*Modelscale;
	}
	for (std::vector<modelNode*>::iterator i = children.begin(); i != children.end(); i++)
	{
		(*i)->update(msec);
	}
	//��ÿ�������ڵ����
}
void modelNode::Draw(shaderProgram* animationshader)
{      
	  
        animationshader->setUniform("model", accumulatetransform);
		if (this->tex!=nullptr)
		{
           this->tex->bindtexture(0);
		   glUniform1i(glGetUniformLocation(animationshader->getprogram(), "Texbuffer0"), 0);//������ͼƬ
		}
		if (Mesh)
		{
			Mesh->draw();
		}

		
}
