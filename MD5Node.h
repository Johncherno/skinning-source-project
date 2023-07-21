#pragma once
#include"MD5FileData.h"
#include"MDK_Anim.h"
#include"shaderProgram.h"
#include<map>
struct bonespaceMatrx
{
	glm::vec3 translation;
	glm::vec3 rotation;
};//�����ڵ�����ռ��¼ӵ�ƽ����ת
class MD5Node
{
public:
	MD5Node( const MD5FileData*MD5file);
	void update(int& frameindex);//���¹��������ؽھ���
	std::string* Get_skeleton_nameArray();
	void init_bonespace_matrix();
	bonespaceMatrx* GetbonespaceMatrix(std::string& Name);//����һ����ַָ�뷽���޸ı������� ���ڴ����������һ������ģ����������
	void update_single_skeletonMatrix(std::string&temp);
	void draw(shaderProgram&shader);//��ʼ��
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
	glm::mat4* bonespaceMatrix;//��������
	std::map<int, std::map<const char*, glm::mat4>>animation_skeleton_Bone_map;//��������ʱ��������ڵ���ƥ�� ����ĳһ���ض�ʱ�������Щ�����ڶ� ��Щ����
};

