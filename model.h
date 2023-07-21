#pragma once
#include<vector>
#include<string>
#include"skeletonMesh.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"aiMatrixGLMconverter.h"
#include<map>
struct SkeletonnodeData
{
	glm::mat4 RelativeToparentTransform;//���ڵ������丸�ڵ�任����
	std::string NodeName;//���ڵ�����
	std::vector<SkeletonnodeData>children;//�洢һϵ���ӽڵ�
};
struct BoneInfo
{
		int id;//���վ��������
		glm::mat4 offset;//ƫ�ƾ���
};
class model:public aiMatrixGLMconverter//��Ҫ��������һ�Ź���ӳ��� ��ȡ����������ض�����Ȩ��  ����������
{
public:
	model(std::string path);//����·��
	void draw();
	auto&GetBoneInfomap(){return BoneInfomap;}//���ع���ӳ���
	unsigned int& GetBoneCount() {return Num_Bones;}//���ع���������
	SkeletonnodeData& GetRootNode();//���ظ��ڵ�
	std::vector<Mesh>meshes;//�洢һϵ������
private:
	Mesh processMesh(aiMesh*mesh);
	void Reading_heirarchyNode(SkeletonnodeData& destination, aiNode* src);//���ؽڵ�ڵ���
	void setVertexBoneDataToDefault(SKeleTonMeshVertex& vertex);//���ù���ת��Ȩ��ֵĬ��ֵ
	void loadModel(std::string path);//����ģ��
	void processNode(aiNode*node,const aiScene*scene);//�����ڵ����ӽڵ�
	void SetVertexBoneData(SKeleTonMeshVertex&vertex,int boneId,float weight);//������������ ����Ȩ��
	void ExtractBoneweightForVertices(std::vector<SKeleTonMeshVertex>& vertices, aiMesh*mesh);
	unsigned int Num_Bones = 0;//����������  
	SkeletonnodeData RootNode;//���ڵ�
	std::map<string, BoneInfo>BoneInfomap;//����Ӧ�����ڵ��Ӧ���������������������Ϣ//BoneInfomap["�첲"]=BoneInfo(�첲�ڵ��Ӧ�ľ���������ƫ�ƾ���)
};


