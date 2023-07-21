#pragma once
#include"Texture2Dimage.h"
#include "mesh.h"
#include"model_Node.h"
#include"NodeDraw.h"
class Bird:public Drawnode
{
public:
	void Create_Bird()
	{    
		Birdbody.loadobj("birdmesh\\birdbody.obj");
		Birdleftwing.loadobj("birdmesh\\leftwing.obj");
		Birdrightwing.loadobj("birdmesh\\rightwing.obj");

		texel.loadtexture("teximage\\bird2.jpg");
	}
	void initBirdNodeMatrix()
	{

		birdleftwing = new modelNode(&Birdleftwing,&texel);
		birdleftwing->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)));
		birdleftwing->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(16.0f, -9.0f, 0)));
		birdleftwing->Set_Node_name("birdleftwing");//小鸟左翅膀

		birdrightwing = new modelNode(&Birdrightwing, &texel);
		birdrightwing->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)));
		birdrightwing->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-12.0f, -8.0f, 0)));
		birdrightwing->Set_Node_name("birdleftwing");//小鸟右翅膀

		birdbody = new modelNode(&Birdbody,&texel);//小鸟身体节点
		birdbody->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(1,1, 1)));
		birdbody->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));
		birdbody->Set_Node_name("birdRoot");
		birdbody->Addchild(birdleftwing);
		birdbody->Addchild(birdrightwing);

		birdRoot = new modelNode();
		birdRoot->Set_modelscale(glm::mat4(1.0f));
		birdRoot->Set_Transform(glm::mat4(1.0f));
		birdRoot->Addchild(birdbody);
	}
	void flying(float deltatime)
	{
		initBirdNodeMatrix();
		birdbody->Set_Transform(birdbody->GetNodetransform()*glm::rotate(glm::mat4(1.0f), 
			glm::radians(wingaccumulation/10.0f), glm::vec3(0, 0, 1)));
		birdleftwing->Set_Transform(birdleftwing->GetNodetransform() 
			* glm::rotate(glm::mat4(1.0f), glm::radians(wingaccumulation), glm::vec3(0, 0, 1)));
		birdrightwing->Set_Transform(birdrightwing->GetNodetransform()
			* glm::rotate(glm::mat4(1.0f), glm::radians(-wingaccumulation), glm::vec3(0, 0, 1)));
	    wingaccumulation += deltatime * 200.0f * direction;
		
		if (wingaccumulation > 74.0f)
		{
			direction = -1.0f;
		}
		else if (wingaccumulation < -10.0f)
		{
			direction = 1.0f;
		}
	}
	modelNode* GetbirdRoot(){return birdRoot;}
private:
	mesh Birdbody;
	mesh Birdleftwing;
	mesh Birdrightwing;

	modelNode* birdbody;
	modelNode* birdleftwing;
	modelNode* birdrightwing;
	modelNode* birdRoot;
	texture2D texel;

	float wingaccumulation = 0.0f;
	float direction = 1.0f;
};