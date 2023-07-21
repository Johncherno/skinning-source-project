#pragma once
#include"glm.hpp"
#include "gtc/matrix_transform.hpp"
#include<string>
#include<map>
#include<vector>
#include <algorithm>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
struct SelfdefineSkeletonVertex
{
	glm::vec3 vertPos;
	glm::vec2 Texelpos;
	float BoneTransformindex[4];//记录每个骨骼索引
	float Weights[4];//每个骨骼对应的权重
};
struct VertaffectedByBoneInfo
{
	unsigned int vertexid;
	float weightValue;
};
struct BoneInformation
{
	glm::mat4 BoneOFFset;
	int boneindex;
	std::string BONEName;
	unsigned int NumVertaffectedthisByBone;
	std::vector<VertaffectedByBoneInfo>VertinfoArrayAffectedByThisBone;
	
};
struct selfBoneNode
{
	glm::mat4 RelativeToparentBindposeTransform;
	glm::mat4 RelativeToparentRotation;
	glm::mat4 RelativeToparenttranslation;
	std::string BoneNodename;
	unsigned int childCount;
	std::vector<selfBoneNode>children;

};
struct SelfdefineSkeletonFaces//顶点面
{
	unsigned int SkeletonIndices[3];
};
class SelfdeFineSkeletonmesh
{
public:
	SelfdeFineSkeletonmesh();
	void DebugVertices();
	void UpdatefinalTransform();
	void DrawourskeletonMesh();
	std::vector<glm::mat4>& Gettrandformation();
private:
	void loadOurskeletonVertices();
	void loadSkeleboneInfo();
	void loadHeirarchyForBoneNode();
	void CalculateOurBoneNodeFinaltransformationForvertices(selfBoneNode*eachNode,glm::mat4 ParentBoneNodeTransform);
	void BindOurMeshToGPU();
	void ExtractBoneweightForourskeletonMeshvertices(std::vector<SelfdefineSkeletonVertex>& selfdefineSkeletonvertices);
	void SetSelfdefineSkeletonVertexBoneData(SelfdefineSkeletonVertex& ourvertex, int boneindex, float WeightValue);
	GLuint Vao, Vbo, Ebo;
	selfBoneNode *lowRightArmBoneNode=nullptr;
	selfBoneNode* UpRightArmBoneNode = nullptr;
	selfBoneNode* selfskeletonRootNode = nullptr;
	std::vector<std::string>BongNameArray;	
	std::vector<SelfdefineSkeletonVertex>ourskeletonMeshvertices;
	std::vector<unsigned int>DrawingIndices;
	std::vector<glm::mat4>FinalTrandformationForVertices;
	std::map<std::string, BoneInformation>skeleBone;
	float time=0;
};

