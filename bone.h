#pragma once
#include<glm.hpp>
#include<vector>
#include<string>
#include"model.h"
#include<iostream>
struct keyposition
{
	glm::vec3 position;//λ��
	float timestamp;//����֡ʱ��
};
struct keyrotation//��ת��Ϣ
{
	glm::quat orientation;//��ת����
	float timestamp;//����֡ʱ��
};
struct keyscale
{
	glm::vec3 scale;//����
	float timestamp;//����֡ʱ��
};
class bone//��Ҫ�����Ǹ��ݶ���ʱ���ȡ����һЩ�任����
{

public:
	bone(const std::string& name, int ID, const aiNodeAnim* channel)//ÿһ��������Ӧһ��ͨ��
		:
		m_Name(name),
		m_ID(ID),
		relativeToparent_Transform(1.0f)//��ʼ�����ǵ�˽�г�Ա��һЩ����   ��Ծ�����Ϊ��λ����
	{
		m_Num_position = channel->mNumPositionKeys;//λ������
		for (int positionindex=0;positionindex<m_Num_position;positionindex++)
		{
			aiVector3D aiposition = channel->mPositionKeys[positionindex].mValue;//����������ڵ㶯��֡��Ӧ������λ�ø�ֵ
			float timestamp = channel->mPositionKeys[positionindex].mTime;//��ȡ����ʱ��֡
			keyposition data;//����һ���ṹ�����
			data.position = GLmget(aiposition);//ת�������ά����
			//std::cout << "X" << data.position.x << " Y" << data.position.y << "Z" << data.position.z << std::endl;
			data.timestamp = timestamp;//��ֵ����������Ķ���ʱ��֡
			m_Position.push_back(data);//������ṹ������Ž�ȥ���������
	    }//��ȡλ��
		m_Num_rotation = channel->mNumRotationKeys;//��ת��Ϣ��ֵ
		for (int rotationindex=0;rotationindex<m_Num_rotation;rotationindex++)
		{
			auto airotation = channel->mRotationKeys[rotationindex].mValue;
			float timestamp = channel->mRotationKeys[rotationindex].mTime;//��ȡ����ʱ��֡
			keyrotation data;//����һ����ת��Ϣ�ṹ�����
			data.orientation = GetGLMQuat(airotation);//��ת��������ǿ��ת��
			data.timestamp = timestamp;//��ת����ʱ��֡
			m_rotation.push_back(data);//������ṹ����������������
		}//��ȡ��ת����
		m_Num_scale = channel->mNumScalingKeys;
		for (int scaleindex=0;scaleindex<m_Num_scale;scaleindex++)
		{
			aiVector3D scale = channel->mScalingKeys[scaleindex].mValue;
			float timestamp = channel->mScalingKeys[scaleindex].mTime;//��ȡ����ʱ��֡
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
		relativeToparent_Transform = glm::rotate(glm::mat4(1.0f), glm::radians(40*animationTime), glm::vec3(1.0f, 0.0f, 0.0f));;//��������ڸ��ڵ�ľ���
	}
	glm::mat4 Getrelative_Toparent() { return relativeToparent_Transform;}//��ȡ����ڸ��ڵ�ľ���
	std::string Getbonename() { return m_Name; }//��ȡ�ڵ����ƥ����
	int GetboneId() { return m_ID; }//���ع������ʾ������������ֵ
	int GetPositionIndex(float animationTime)//ͨ������ʱ���ȡ�任λ������
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
	int GetRotationIndex(float animationTime)//ͨ������ʱ������ȡ��ת��������ֵ
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
	int GetScaleIndex(float animationTime)////ͨ������ʱ������ȡ���ž�������ֵ
	{
		for (int index = 0; index < m_Num_scale - 1; index++)
		{
			if (animationTime <= m_scale[index + 1].timestamp)
				return index; break;                                                       
		}
		//assert(0);
	}

private:
	std::vector<keyposition>m_Position;//���λ��ƫ�Ƶ�����
	std::vector<keyrotation>m_rotation;//�����ת��Ϣ������
	std::vector<keyscale>m_scale;//���������Ϣ������
	int m_Num_position;	  //����λ������
	int m_Num_rotation;	  //������ת��Ϣ��
	int m_Num_scale;	  //������������
	glm::mat4 relativeToparent_Transform;//��Ը��ڵ����Ծ���
	std::string m_Name;//�ļ����ַ���
	int m_ID;//Id������
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
	}//��ȡǰ�󶯻�֡�������
	glm::mat4 Interpolateposition(float animationTime)
	{
		if (1 == m_Num_position) { return glm::translate(glm::mat4(1.0f), m_Position[0].position); }
		int p0index = GetPositionIndex(animationTime);//���ݶ���ʱ���ȡλ��ƫ��
		int p1index = p0index + 1;
		float scalefactor = GetScaleFactor(m_Position[p0index].timestamp, m_Position[p1index].timestamp,animationTime);
		glm::vec3 finalposition = glm::mix(m_Position[p0index].position, m_Position[p1index].position, scalefactor);//��ȡ����λ��
		//std::cout << "����ʱ��" << animationTime << std::endl;
		//std::cout << finalposition.x << " " << finalposition.x << " " << finalposition.x << " " << std::endl;
		//std::cout << "����ʱ��" << animationTime  << std::endl;
		return glm::translate(glm::mat4(1.0f), finalposition);//��ȡ���յ�λ��
		
	}
	glm::mat4 Interpolaterotation(float animationTime)
	{
		int p0index = GetRotationIndex(animationTime);//��ȡ��ת��������ֵ
		int p1index = p0index + 1;
		float scalefactor = GetScaleFactor(m_rotation[p0index].timestamp, m_rotation[p1index].timestamp, animationTime);
		//std::cout << scalefactor << std::endl;
		glm::quat finalrotation = glm::slerp(m_rotation[p0index].orientation, m_rotation[p1index].orientation, scalefactor);
		finalrotation = glm::normalize(finalrotation);
		glm::mat4 temp = glm::mat4(finalrotation);
		//std::cout << "��ת����" << temp[0][0] << " " << temp[0][1] << " " << temp[0][2] << " " << temp[0][3] << " " << std::endl;
		//std::cout << "��ת����" << temp[1][0] << " " << temp[1][1] << " " << temp[1][2] << " " << temp[1][3] << " " << std::endl;
		//std::cout << "��ת����" << temp[2][0] << " " << temp[2][1] << " " << temp[2][2] << " " << temp[2][3] << " " << std::endl;
		//std::cout << "��ת����" << temp[3][0] << " " << temp[3][1] << " " << temp[3][2] << " " << temp[3][3] << " " << std::endl;
		return glm::mat4(finalrotation);
		
	}//������ת����
	glm::mat4 Interpolatescale(float animationTime)
	{
		int p0index = GetScaleIndex(animationTime);
		int p1index = p0index + 1;
		float scalefactor = GetScaleFactor(m_scale[p0index].timestamp,m_scale[p1index].timestamp,animationTime);
		glm::vec3  Scale = glm::mix(m_scale[p0index].scale,m_scale[p1index].scale,scalefactor);
		return glm::scale(glm::mat4(1.0f), Scale);//��ȡ���ž���
		
	}
};

