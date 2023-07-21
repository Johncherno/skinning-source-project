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
};//���Ӽ̳й�ϵ
struct MD5_frame
{
	float* component;
};
struct Base
{
	glm::vec3* position;//һϵ�й����ڵ��ʼλ�ú���ת��
	glm::vec3* orientation;
};
class MDK_Anim
{
public:
	friend class MD5Node;

	MDK_Anim(const std::string&filename);
private:
	void Loadhirearchy(std::ifstream&from,int&count);//���ظ��ӹ�ϵ
	void LoadAnimateframe(std::ifstream& from,int&count);//���ض���֡
	void LoadAnimateBasepos(std::ifstream& from);//���س�ʼλ��
	void transform_skeleton(MD5Skeleton&skeleton,int framenum);
	void generateW(glm::vec3& p);
	glm::mat4 animationpos_toRotationMatrix(glm::vec3&p);
	int numJoints;//�����ؽڽڵ�����
	int num_animate_components;//��������Ҫ��������Ԫ��
	int num_frames;//����֡����
	int framerate;//��������
	float W;
	MD5_ANIM_Joint* Joint;//�����ڵ�����
	MD5_frame* animated_frame;
	Base basepos;
};

