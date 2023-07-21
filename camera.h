#pragma once
#include"glm.hpp"
class camera
{
public:
	glm::mat4 getViewMatrix()const;
	virtual void rotate(float yaw, float pitch);
protected:
	camera();
	glm::vec3 mPosition;//相机位置
	glm::vec3 mTargetpos;//目标位置
	glm::vec3 mUp;//向上方向向量
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
