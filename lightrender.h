#pragma once
#include "shaderProgram.h"
#include "mesh.h"
#include<glm.hpp>
class lightrender
{
public:
	void init();
	void lightblurbindFBO();
	void render(glm::vec3&camera,glm::vec3& lightpos,glm::vec4&lightcolor,glm::vec3&direction);
private:
	shaderProgram lightshader;
	mesh lightmesh;
	unsigned int BlurFBO;//÷°ª∫≥Â
	unsigned int colorbuffer;//Œ∆¿ÌÕº
};

