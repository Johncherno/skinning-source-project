#pragma once
#include<glm.hpp>
#include<iostream>
#include<sstream>
#include<fstream>
#include"MD5FileData.h"

#define MD5_ANIM_XPOS			1
#define MD5_ANIM_YPOS			2
#define MD5_ANIM_ZPOS			4

#define MD5_ANIM_XQUAT			8
#define MD5_ANIM_YQUAT			16
#define MD5_ANIM_ZQUAT			32


struct MD5_ANIM_Joint
{
	std::string name;
	int parent_index;
	int component_flag;
	int start_index;
};//父子继承关系
struct MD5_frame
{
	float* component;
};
struct Base
{
	glm::vec3* position;//一系列骨骼节点初始位置和旋转角
	glm::vec3* orientation;
};
class MDK_Anim
{
public:
	friend class MD5Node;

	MDK_Anim(const std::string&filename);
private:
	void Loadhirearchy(std::ifstream&from,int&count);//加载父子关系
	void LoadAnimateframe(std::ifstream& from,int&count);//加载动画帧
	void LoadAnimateBasepos(std::ifstream& from);//加载初始位置
	void transform_skeleton(MD5Skeleton&skeleton,int framenum);
	void generateW(glm::vec3& p);
	glm::mat4 animationpos_toRotationMatrix(glm::vec3&p);
	int numJoints;//骨骼关节节点数量
	int num_animate_components;//骨骼动画要动的向量元素
	int num_frames;//多少帧动画
	int framerate;//动画速率
	float W;
	MD5_ANIM_Joint* Joint;//骨骼节点数组
	MD5_frame* animated_frame;
	Base basepos;
};

