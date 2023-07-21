#pragma once
#include"glm.hpp"
class camera
{
public:
	glm::mat4 getViewMatrix()const;
	virtual void rotate(float yaw, float pitch);
protected:
	camera();
	glm::vec3 mPosition;//���λ��
	glm::vec3 mTargetpos;//Ŀ��λ��
	glm::vec3 mUp;//���Ϸ�������
	float mYaw;
	float mPitch;

};
class Orbitcamera :public camera
{
	friend camera;
public:
	Orbitcamera();
	virtual void rotate(float yaw,float pitch);
	void setLookat(const glm::vec3&target);
	void setRadius(float radius);
private:
	void updatacameravector();
	float mRadius;
};
