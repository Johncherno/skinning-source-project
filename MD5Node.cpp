#include "MD5Node.h"

MD5Node::MD5Node( const MD5FileData* MD5file)
	:sourcedata(*MD5file)
{
	sourcedata.clone_skeletoninformation(Rootskeleton);
	
}

void MD5Node::update(int& frameindex)
{
	if (currentAnim)
	{
		if (frameindex>=currentAnim->num_frames)
		{
			frameindex = 0;
	    }
		currentAnim->transform_skeleton(Rootskeleton,frameindex);
		/*frameindex=frameindex++% currentAnim->num_frames;*/
	}
}



std::string* MD5Node::Get_skeleton_nameArray()
{   
	std::string* pstring = new std::string[currentAnim->numJoints];
	for (int jointindex = 0; jointindex < currentAnim->numJoints; jointindex++)
	{
		pstring[jointindex] = Rootskeleton.joints[jointindex].name;
	}
	return pstring;
}

void MD5Node::init_bonespace_matrix()
{

	try
	{
        BonespaceMatrix = new bonespaceMatrx[currentAnim->numJoints];
		bonespaceMatrix= new glm::mat4[currentAnim->numJoints];
	 }
	catch (std::bad_alloc)
	{
		std::cout << "内存已满" << std::endl;
	}
	for (int jointindex = 0; jointindex < currentAnim->numJoints; jointindex++)
	{

		BonespaceMatrix[jointindex].translation = glm::vec3(0, 0, 0);
		BonespaceMatrix[jointindex].rotation = glm::vec3(0, 0, 0);
	}
}

bonespaceMatrx* MD5Node::GetbonespaceMatrix(std::string&Name)
{   

	for (int jointindex = 0; jointindex < currentAnim->numJoints; jointindex++)
	{
		if (Rootskeleton.joints[jointindex].name == Name)
		{
			return &BonespaceMatrix[jointindex];
		}
	}
	
}



void MD5Node::update_single_skeletonMatrix(std::string&temp)
{    
	for (int jointindex = 0; jointindex < currentAnim->numJoints; jointindex++)
	{
		glm::mat4 tempmodel(1.0f);
		tempmodel= glm::translate(tempmodel,BonespaceMatrix[jointindex].translation) * 
			glm::rotate(tempmodel, glm::radians(BonespaceMatrix[jointindex].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))*
			glm::rotate(tempmodel, glm::radians(BonespaceMatrix[jointindex].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))*
			glm::rotate(tempmodel, glm::radians(BonespaceMatrix[jointindex].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		if (Rootskeleton.joints[jointindex].name==temp)
		{
			Rootskeleton.joints[jointindex].joint_transform = Rootskeleton.joints[jointindex].initial_Joint_Matrix*tempmodel;
		}
		accumulate_hirearchy_transform(jointindex);//累乘父子节点矩阵
	}
}

void MD5Node::draw(shaderProgram& shader)
{
	//sourcedata.CreateTBO();
	sourcedata.BindTextureBuffers();
	sourcedata.update_transformTBO(Rootskeleton);//将已经通过骨骼动画更新的骨骼节点矩阵放到矩阵纹理缓冲上来
	//sourcedata.updateTransform(Rootskeleton);
	glUniform1i(glGetUniformLocation(shader.getprogram(), "weightTex"), 0);
	glUniform1i(glGetUniformLocation(shader.getprogram(), "transformTex"), 1);
	sourcedata.Draw();//开始画我们的骨骼动画
}
//
//weightTex;//权
//transformTex;

void MD5Node::AddAnim(const std::string& filename)
{
	currentAnim=new MDK_Anim(filename);//分配骨骼动画地址
}

void MD5Node::Setanimation_skeleton_Bone_map(int frameindex, std::string& skeletonname,glm::mat4 skeleton_modify_transform)
{
	animation_skeleton_Bone_map[frameindex][skeletonname.c_str()] = skeleton_modify_transform;
}

void MD5Node::playanim(int animation_Time_index)
{
	for (int jointindex = 0; jointindex < currentAnim->numJoints; jointindex++)//遍历所有骨骼节点
	{
		if (animation_skeleton_Bone_map[animation_Time_index].find(Rootskeleton.joints[jointindex].name.c_str()) 
			!= animation_skeleton_Bone_map[animation_Time_index].end())
			//如果从这帧中发现这个骨骼节点在动
		{
			bonespaceMatrix[jointindex] = animation_skeleton_Bone_map[animation_Time_index][Rootskeleton.joints[jointindex].name.c_str()];//那我们就更新这个数组变量
		}
	}

}
//skeleton->playanim(Time_index)
//skeleton->update_boneMatrix()
//Time_index++
void MD5Node::update_boneMatrix()
{
	for (int jointindex = 0; jointindex < currentAnim->numJoints; jointindex++)
	{
		Rootskeleton.joints[jointindex].joint_transform = Rootskeleton.joints[jointindex].initial_Joint_Matrix * bonespaceMatrix[jointindex];
		accumulate_hirearchy_transform(jointindex);
	}
}

void MD5Node::accumulate_hirearchy_transform(int jointinndex)
{
	if (Rootskeleton.joints[jointinndex].parent_index < 0)//它本身就是父类
	{
		Rootskeleton.joints[jointinndex].accumulate_transform = Rootskeleton.joints[jointinndex].joint_transform;
	}
	else//它不是父类 是子类
	{

		Rootskeleton.joints[jointinndex].accumulate_transform =
			Rootskeleton.joints[Rootskeleton.joints[jointinndex].parent_index].accumulate_transform * Rootskeleton.joints[jointinndex].joint_transform;//子类矩阵再乘上父类累乘矩阵
	}
}
