#include "model_Node.h"
#include<iostream>
modelNode::modelNode( mesh* Mesh=nullptr, texture2D* Tex=nullptr)
	:accumulatetransform(glm::mat4(1.0f)),
	 Modelscale(glm::mat4(1.0f)),
	 transform(glm::mat4(1.0f))
{
	this->Mesh = Mesh;//拷贝到这个类的网格
	this->tex = Tex;//拷贝到这个类的纹理
	this->parent = nullptr;
	
}
modelNode::modelNode()
	:accumulatetransform(glm::mat4(1.0f)), 
	 Modelscale(glm::mat4(1.0f)), 
	 transform(glm::mat4(1.0f))
{
	this->Mesh = nullptr;//拷贝到这个类的网格
	this->tex = nullptr;//拷贝到这个类的纹理
	this->parent = nullptr;
	
}
void modelNode::Set_modelscale(const glm::mat4& modelscale)
{
	Modelscale = modelscale;
}
void modelNode::Set_Transform(const glm::mat4& translation) {
	transform = translation ;//设置自己的节点的变换矩阵
}
void modelNode::Addchild(modelNode* child) {
	children.push_back(child);
	child->parent = this;
}
void modelNode::update(float msec)
{    
	//std::cout<<"we are excuting modelnode programing" << msec << std::endl;
	
	if (parent )//你不是父节点
	{
		accumulatetransform = parent->accumulatetransform * transform*Modelscale;//你自己的变换矩阵乘上上一级父亲节点的矩阵
		//std::cout << name << std::endl;
	}
	else//那么你就是父节点
	{
		accumulatetransform = transform*Modelscale;
	}
	for (std::vector<modelNode*>::iterator i = children.begin(); i != children.end(); i++)
	{
		(*i)->update(msec);
	}
	//对每个骨骼节点更新
}
void modelNode::Draw(shaderProgram* animationshader)
{      
	  
        animationshader->setUniform("model", accumulatetransform);
		if (this->tex!=nullptr)
		{
           this->tex->bindtexture(0);
		   glUniform1i(glGetUniformLocation(animationshader->getprogram(), "Texbuffer0"), 0);//绑定纹理图片
		}
		if (Mesh)
		{
			Mesh->draw();
		}

		
}
