#pragma once
#include"MD5FileData.h"
#include"MDK_Anim.h"
#include"shaderProgram.h"
#include<map>
struct bonespaceMatrx
{
	glm::vec3 translation;
	glm::vec3 rotation;
};//骨骼节点骨骼空间下加的平移旋转
class MD5Node
{
public:
	MD5Node( const MD5FileData*MD5file);
	void update(int& frameindex);//更新骨骼动画关节矩阵
	std::string* Get_skeleton_nameArray();
	void init_bonespace_matrix();
	bonespaceMatrx* GetbonespaceMatrix(std::string& Name);//返回一个地址指针方便修改变量数据 便于传入参数到下一个函数模块引导骨骼
	void update_single_skeletonMatrix(std::string&temp);
	void draw(shaderProgram&shader);//开始画
	void AddAnim(const std::string&filename);
	void Setanimation_skeleton_Bone_map(int frameindex,std::string&skeletonname, glm::mat4 skeleton_modify_transform);
	void playanim(int animation_Time_index);
	void update_boneMatrix();

private:
	void accumulate_hirearchy_transform(int jointinndex);
	const MD5FileData& sourcedata;
	MD5Skeleton Rootskeleton;
	MDK_Anim* currentAnim;
	bonespaceMatrx* BonespaceMatrix;
	glm::mat4* bonespaceMatrix;//矩阵数组
	std::map<int, std::map<const char*, glm::mat4>>animation_skeleton_Bone_map;//骨骼动画时间与骨骼节点相匹配 代表某一个特定时间点内那些骨骼在动 那些不动
};

