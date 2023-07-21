#pragma once
#include"model.h"
class Animator
{
public:
	Animator(model* ourModel);
	void UpdateAnimation(float dt);
	void Bindtexbuffer();
private:
	void Calculatebonetransform(SkeletonnodeData* node, glm::mat4 parenttransform);
	void create_transformbuffer_tex();
	void updateskeletonTBO();
	std::vector<glm::mat4> m_FinalBoneMatrices;
	model* skeleModel = nullptr;
	float CurrentTime = 0.0f;
	GLuint skeletontransformBuffer;//æÿ’Ûª∫≥Â
	GLuint skeletontransformTexture;//æÿ’ÛŒ∆¿Ì
};

