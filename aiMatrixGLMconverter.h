#pragma once
#include<glm.hpp>
#include<assimp/postprocess.h>
#include<assimp/matrix4x4.h>
class aiMatrixGLMconverter
{
public:
	glm::mat4 AiMatrixGLMconverter(aiMatrix4x4& from);
};

