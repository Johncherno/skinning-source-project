#pragma once
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include "shaderProgram.h"
class shadowmap
{
public:
	void Shadowmapinit();//阴影深度初始化
	void Shadowmapbindfbo();//绑定阴影帧缓冲
	glm::mat4 Shadowmapsetlight(glm::vec3&lightposition,float&left,float&right,float&bottom,float&top,float&near,float&far);//设置光线视野矩阵
	void setsamplershadowmap(shaderProgram*shader);
private:
	unsigned int shadowmapFBO;//帧缓冲
	unsigned int shadowmap;//阴影映射
	glm::mat4 lightP, lightV;
	glm::mat4 lightspaceM;
};

