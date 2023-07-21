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
	glm::mat4 RelativeToparentTransform;//本节点绕着其父节点变换矩阵
	std::string NodeName;//本节点名字
	std::vector<SkeletonnodeData>children;//存储一系列子节点
};
struct BoneInfo
{
		int id;//最终矩阵的索引
		glm::mat4 offset;//偏移矩阵
};
class model:public aiMatrixGLMconverter//主要工作生成一张骨骼映射表 获取顶点相对于特定骨骼权重  骨骼总数量
{
public:
	model(std::string path);//加载路径
	void draw();
	auto&GetBoneInfomap(){return BoneInfomap;}//返回骨骼映射表
	unsigned int& GetBoneCount() {return Num_Bones;}//返回骨骼总数量
	SkeletonnodeData& GetRootNode();//返回根节点
	std::vector<Mesh>meshes;//存储一系列网格
private:
	Mesh processMesh(aiMesh*mesh);
	void Reading_heirarchyNode(SkeletonnodeData& destination, aiNode* src);//加载节点节点树
	void setVertexBoneDataToDefault(SKeleTonMeshVertex& vertex);//设置骨骼转换权重值默认值
	void loadModel(std::string path);//加载模型
	void processNode(aiNode*node,const aiScene*scene);//处理父节点与子节点
	void SetVertexBoneData(SKeleTonMeshVertex&vertex,int boneId,float weight);//拷贝骨骼索引 拷贝权重
	void ExtractBoneweightForVertices(std::vector<SKeleTonMeshVertex>& vertices, aiMesh*mesh);
	unsigned int Num_Bones = 0;//骨骼计数器  
	SkeletonnodeData RootNode;//根节点
	std::map<string, BoneInfo>BoneInfomap;//用相应骨骼节点对应的名字来访问这个骨骼信息//BoneInfomap["胳膊"]=BoneInfo(胳膊节点对应的矩阵索引和偏移矩阵)
};


