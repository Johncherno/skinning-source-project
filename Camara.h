#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<glm.hpp>
class Camara
{
public:
	void UpdataCamerapos(GLFWwindow*window);//更新摄像机位置a
	void UpdateCameraFront(float&yaw,float&pitch)
	{
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction); 
	}
	
	glm::vec3 GetCameraFront() { return cameraFront;}//获取向前方向向量
	//float Yaw, Pitch = 0.0f;//摄像机视野旋转角
	glm::vec3 position=glm::vec3(0.0f,0.0f,1.0f);//摄像机位置
	
private:
	glm::vec3 direction;//方向向量
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//摄像机向前方向向量
	glm::vec3 up=glm::vec3(0.0f, 1.0f, 0.0f);//向上向量
};

