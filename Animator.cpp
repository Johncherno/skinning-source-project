#include "Animator.h"

Animator::Animator(model* ourModel)
{
	skeleModel = ourModel;
	for (int i = 0; i < ourModel->GetBoneCount(); i++)
		m_FinalBoneMatrices.push_back(glm::mat4(1.0f));//初始化这个矩阵数据
	create_transformbuffer_tex();
}

void Animator::UpdateAnimation(float dt)
{
	CurrentTime += dt;
	Calculatebonetransform(&skeleModel->GetRootNode(), glm::mat4(1.0f));
	updateskeletonTBO();//更新矩阵数组

}

void Animator::Calculatebonetransform(SkeletonnodeData* node, glm::mat4 parenttransform)
{   
	std::string bonename = node->NodeName;
	glm::mat4   ApplyRotationTo_RelativeToParent_transform;

	if (bonename == "w1_L" || bonename == "w2_L" || bonename == "w3_L" || bonename == "w4_L" || bonename == "w5_L"
		|| bonename == "w6_L"
		|| bonename == "w7_L"
		|| bonename == "w8_L"
		|| bonename == "w9_L")
	{
		ApplyRotationTo_RelativeToParent_transform = node->RelativeToparentTransform * glm::rotate(glm::mat4(1.0f), glm::radians(35 * sin(CurrentTime * 3.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (bonename == "w1_R" || bonename == "w2_R" || bonename == "w3_R" || bonename == "w4_R" || bonename == "w5_R"
		|| bonename == "w6_R"
		|| bonename == "w7_R"
		|| bonename == "w8_R"
		|| bonename == "w9_R")
	{
		ApplyRotationTo_RelativeToParent_transform = node->RelativeToparentTransform * glm::rotate(glm::mat4(1.0f), glm::radians(-35 * sin(CurrentTime * 3.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (bonename == "head")
	{
		ApplyRotationTo_RelativeToParent_transform = node->RelativeToparentTransform * glm::rotate(glm::mat4(1.0f), glm::radians(16 * sin(CurrentTime * 6.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
	}
	else if (bonename == "ACT_neck_1" || bonename == "ACT_neck_2" || bonename == "ACT_neck_3" || bonename == "ACT_neck_4")
	{
		ApplyRotationTo_RelativeToParent_transform = node->RelativeToparentTransform * glm::rotate(glm::mat4(1.0f), glm::radians(3 * sin(CurrentTime * 6.0f)), glm::vec3(1.0f, 0.0f, 1.0f));
	}
	else if (bonename == "Bone")
	{
		ApplyRotationTo_RelativeToParent_transform = node->RelativeToparentTransform * glm::rotate(glm::mat4(1.0f), glm::radians(34 * fabsf(sin(CurrentTime * 3.0f))), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	//upper_arm_L  upper_arm_R  ik_underarm_L
	else if (bonename == "upper_arm_L" || bonename == "lowerleg_L" || bonename == "Hand_L")
	{
		ApplyRotationTo_RelativeToParent_transform = node->RelativeToparentTransform * glm::rotate(glm::mat4(1.0f), glm::radians(14 * sin(CurrentTime * 6.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (bonename == "upper_arm_R" || bonename == "lowerleg_R" || bonename == "Hand_R")
	{
		ApplyRotationTo_RelativeToParent_transform = node->RelativeToparentTransform * glm::rotate(glm::mat4(1.0f), glm::radians(14 * sin(-CurrentTime * 6.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else if (bonename == "tail_1")//ACT_neck_1
	{
		ApplyRotationTo_RelativeToParent_transform = node->RelativeToparentTransform * glm::rotate(glm::mat4(1.0f), glm::radians(16 * sin(CurrentTime * 6.0f)), glm::vec3(1.0f, 0.0f, 1.0f));
	}
	else
	{
		ApplyRotationTo_RelativeToParent_transform = node->RelativeToparentTransform;
	}
	auto boneinfomap = skeleModel->GetBoneInfomap();//通过这个骨骼动画获取骨骼映射表
	glm::mat4 accumulatetransform = parenttransform * ApplyRotationTo_RelativeToParent_transform;//累乘矩阵
	if (boneinfomap.find(bonename) != boneinfomap.end())
	{
		glm::mat4 BoneOffsetTransform = boneinfomap[bonename].offset;
		int index = boneinfomap[bonename].id;//获取索引
		//std::cout << "骨骼节点名字" << bonename << std::endl;
		m_FinalBoneMatrices[index] = accumulatetransform * BoneOffsetTransform; //累乘矩阵乘上偏移矩阵 转换到骨骼空间上旋转
	}
	for (SkeletonnodeData child : node->children)
	{
		Calculatebonetransform(&child, accumulatetransform);//不断递归遍历所有子节点
	}

}

void Animator::create_transformbuffer_tex()
{
	glGenBuffers(1, &skeletontransformBuffer);
	glGenTextures(1, &skeletontransformTexture);
	glBindBuffer(GL_TEXTURE_BUFFER, skeletontransformBuffer);
	glBufferData(GL_TEXTURE_BUFFER, m_FinalBoneMatrices.size() * sizeof(glm::mat4), &m_FinalBoneMatrices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
}

void Animator::Bindtexbuffer()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, skeletontransformTexture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, skeletontransformBuffer);
}

void Animator::updateskeletonTBO()
{
	glBindBuffer(GL_TEXTURE_BUFFER, skeletontransformBuffer);
	glBufferSubData(GL_TEXTURE_BUFFER, 0, m_FinalBoneMatrices.size() * sizeof(glm::mat4), (void*)&m_FinalBoneMatrices[0]);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
}
