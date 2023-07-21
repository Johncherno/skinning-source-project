#pragma once
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include "shaderProgram.h"
class shadowmap
{
public:
	void Shadowmapinit();//��Ӱ��ȳ�ʼ��
	void Shadowmapbindfbo();//����Ӱ֡����
	glm::mat4 Shadowmapsetlight(glm::vec3&lightposition,float&left,float&right,float&bottom,float&top,float&near,float&far);//���ù�����Ұ����
	void setsamplershadowmap(shaderProgram*shader);
private:
	unsigned int shadowmapFBO;//֡����
	unsigned int shadowmap;//��Ӱӳ��
	glm::mat4 lightP, lightV;
	glm::mat4 lightspaceM;
};

