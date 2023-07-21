#pragma once
#include<glm.hpp>
#include<vector>
#include<string>
#include"model.h"
#include<iostream>
struct keyposition
{
	glm::vec3 position;//位置
	float timestamp;//动画帧时间
};
struct keyrotation//旋转信息
{
	glm::quat orientation;//旋转矩阵
	float timestamp;//动画帧时间
};
struct keyscale
{
	glm::vec3 scale;//缩放
	float timestamp;//动画帧时间
};
class bone//主要工作是根据动画时间获取骨骼一些变换矩阵
{

public:
	bone(const std::string& name, int ID, const aiNodeAnim* channel)//每一个骨骼对应一个通道
		:
		m_Name(name),
		m_ID(ID),
		relativeToparent_Transform(1.0f)//初始化我们的私有成员的一些变量   相对矩阵设为单位矩阵
	{
		m_Num_position = channel->mNumPositionKeys;//位置数量
		for (int positionindex=0;positionindex<m_Num_position;positionindex++)
		{
			aiVector3D aiposition = channel->mPositionKeys[positionindex].mValue;//将这个骨骼节点动画帧相应的索引位置赋值
			float timestamp = channel->mPositionKeys[positionindex].mTime;//获取动画时间帧
			keyposition data;//定义一个结构体变量
			data.position = GLmget(aiposition);//转换这个三维向量
			//std::cout << "X" << data.position.x << " Y" << data.position.y << "Z" << data.position.z << std::endl;
			data.timestamp = timestamp;//赋值给这个变量的动画时间帧
			m_Position.push_back(data);//将这个结构体变量放进去这个容器中
	    }//获取位置
		m_Num_rotation = channel->mNumRotationKeys;//旋转信息赋值
		for (int rotationindex=0;rotationindex<m_Num_rotation;rotationindex++)
		{
			auto airotation = channel->mRotationKeys[rotationindex].mValue;
			float timestamp = channel->mRotationKeys[rotationindex].mTime;//获取动画时间帧
			keyrotation data;//定义一个旋转信息结构体变量
			data.orientation = GetGLMQuat(airotation);//旋转矩阵类型强制转换
			data.timestamp = timestamp;//旋转动画时间帧
			m_rotation.push_back(data);//将这个结构体变量返给这个容器
		}//获取旋转矩阵
		m_Num_scale = channel->mNumScalingKeys;
		for (int scaleindex=0;scaleindex<m_Num_scale;scaleindex++)
		{
			aiVector3D scale = channel->mScalingKeys[scaleindex].mValue;
			float timestamp = channel->mScalingKeys[scaleindex].mTime;//获取动画时间帧
			keyscale data;
			data.scale = GLmget(scale);
			data.timestamp = timestamp;
			m_scale.push_back(data);
			
		}
	}
	void update(float animationTime)
	{
		glm::mat4 translation = Interpolateposition(animationTime);
		glm::mat4 rotation = Interpolaterotation(animationTime);
		glm::mat4 scale = Interpolatescale(animationTime);
		relativeToparent_Transform = glm::rotate(glm::mat4(1.0f), glm::radians(40*animationTime), glm::vec3(1.0f, 0.0f, 0.0f));;//返回相对于父节点的矩阵
	}
	glm::mat4 Getrelative_Toparent() { return relativeToparent_Transform;}//获取相对于父节点的矩阵
	std::string Getbonename() { return m_Name; }//获取节点骨骼匹配名
	int GetboneId() { return m_ID; }//返回骨骼访问矩阵数组的索引值
	int GetPositionIndex(float animationTime)//通过动画时间获取变换位置索引
	{
		for (int index = 0; index < m_Num_position- 1; index++)
		{
			if (animationTime <= m_Position[index + 1].timestamp)
				//std::cout << "position next time" << m_Position[index + 1].timestamp << std::endl;
				return index; break;
			//
		}
		//assert(0);
	}

	/* Gets the current index on mKeyRotations to interpolate to based on the
	current animation time*/
	int GetRotationIndex(float animationTime)//通过动画时间来获取旋转矩阵索引值
	{
		for (int index = 0; index < m_Num_rotation - 1; index++)
		{     
			if (animationTime <= m_rotation[index + 1].timestamp)
				//std::cout << animationTime << std::endl;
				//
				return index; break;
		}
		//assert(0);
	}

	/* Gets the current index on mKeyScalings to interpolate to based on the
	current animation time */
	int GetScaleIndex(float animationTime)////通过动画时间来获取缩放矩阵索引值
	{
		for (int index = 0; index < m_Num_scale - 1; index++)
		{
			if (animationTime <= m_scale[index + 1].timestamp)
				return index; break;                                                       
		}
		//assert(0);
	}

private:
	std::vector<keyposition>m_Position;//存放位置偏移的容器
	std::vector<keyrotation>m_rotation;//存放旋转信息的容器
	std::vector<keyscale>m_scale;//存放缩放信息的容器
	int m_Num_position;	  //动画位置数量
	int m_Num_rotation;	  //动画旋转信息数
	int m_Num_scale;	  //动画缩放数量
	glm::mat4 relativeToparent_Transform;//相对父节点的相对矩阵
	std::string m_Name;//文件名字符串
	int m_ID;//Id索引号
	aiVector3D GLMtoaiVec(const glm::vec3& vec)
	{
		return aiVector3D(vec.x, vec.y, vec.z);
	}
	glm::vec3 GLmget(const aiVector3D&vec)
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	}
	glm::mat4 aiQuternionGLMconverter(aiMatrix4x4& from)
	{
		glm::mat4 to(1.0f);
		to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;

		return to;
	}
	glm::quat GetGLMQuat(const aiQuaternion &pOrientation)
	{
		return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
	}
	/* Gets normalized value for Lerp & Slerp*/
	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		float scaleFactor = 0.0f;
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;
		scaleFactor = midWayLength / framesDiff;
		return scaleFactor;
	}//获取前后动画帧混合因子
	glm::mat4 Interpolateposition(float animationTime)
	{
		if (1 == m_Num_position) { return glm::translate(glm::mat4(1.0f), m_Position[0].position); }
		int p0index = GetPositionIndex(animationTime);//根据动画时间获取位置偏移
		int p1index = p0index + 1;
		float scalefactor = GetScaleFactor(m_Position[p0index].timestamp, m_Position[p1index].timestamp,animationTime);
		glm::vec3 finalposition = glm::mix(m_Position[p0index].position, m_Position[p1index].position, scalefactor);//获取最终位置
		//std::cout << "动画时间" << animationTime << std::endl;
		//std::cout << finalposition.x << " " << finalposition.x << " " << finalposition.x << " " << std::endl;
		//std::cout << "动画时间" << animationTime  << std::endl;
		return glm::translate(glm::mat4(1.0f), finalposition);//获取最终的位置
		
	}
	glm::mat4 Interpolaterotation(float animationTime)
	{
		int p0index = GetRotationIndex(animationTime);//获取旋转矩阵索引值
		int p1index = p0index + 1;
		float scalefactor = GetScaleFactor(m_rotation[p0index].timestamp, m_rotation[p1index].timestamp, animationTime);
		//std::cout << scalefactor << std::endl;
		glm::quat finalrotation = glm::slerp(m_rotation[p0index].orientation, m_rotation[p1index].orientation, scalefactor);
		finalrotation = glm::normalize(finalrotation);
		glm::mat4 temp = glm::mat4(finalrotation);
		//std::cout << "旋转矩阵" << temp[0][0] << " " << temp[0][1] << " " << temp[0][2] << " " << temp[0][3] << " " << std::endl;
		//std::cout << "旋转矩阵" << temp[1][0] << " " << temp[1][1] << " " << temp[1][2] << " " << temp[1][3] << " " << std::endl;
		//std::cout << "旋转矩阵" << temp[2][0] << " " << temp[2][1] << " " << temp[2][2] << " " << temp[2][3] << " " << std::endl;
		//std::cout << "旋转矩阵" << temp[3][0] << " " << temp[3][1] << " " << temp[3][2] << " " << temp[3][3] << " " << std::endl;
		return glm::mat4(finalrotation);
		
	}//返回旋转矩阵
	glm::mat4 Interpolatescale(float animationTime)
	{
		int p0index = GetScaleIndex(animationTime);
		int p1index = p0index + 1;
		float scalefactor = GetScaleFactor(m_scale[p0index].timestamp,m_scale[p1index].timestamp,animationTime);
		glm::vec3  Scale = glm::mix(m_scale[p0index].scale,m_scale[p1index].scale,scalefactor);
		return glm::scale(glm::mat4(1.0f), Scale);//获取缩放矩阵
		
	}
};

