#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm.hpp>
class Camara
{
public:
	void UpdataCamerapos(GLFWwindow*window);//���������λ��a
	void UpdateCameraFront(float&yaw,float&pitch)
	{
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction); 
	}
	
	glm::vec3 GetCameraFront() { return cameraFront;}//��ȡ��ǰ��������
	//float Yaw, Pitch = 0.0f;//�������Ұ��ת��
	glm::vec3 position=glm::vec3(0.0f,0.0f,1.0f);//�����λ��
	
private:
	glm::vec3 direction;//��������
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//�������ǰ��������
	glm::vec3 up=glm::vec3(0.0f, 1.0f, 0.0f);//��������
};

