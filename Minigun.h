#pragma once
#include"Texture2Dimage.h"
#include "mesh.h"
#include"model_Node.h"
#include"NodeDraw.h"
#define gunbulletcount 3
class Minigun:public Drawnode
{
public:
	void Create_MiniGun()//创建整个枪
	{
		Gunbody.loadobj("mesh\\gun.obj");
		Gunslider.loadobj("mesh\\gunslider.obj");
		Gunbullet.loadobj("mesh\\bullet.obj");
		text[0].loadtexture("teximage\\blackgun.jpg");
		text[1].loadtexture("teximage\\duol.png");
		text[2].loadtexture("teximage\\metal.jpg");
	}
	   
	void initGunNodeMatrix()//初始化枪每个节点的变换矩阵
	{
		gunbody= new modelNode(&Gunbody, &text[2]);//模型节点地址
		gunbody->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(0.8, 0.7, 0.7)));
		gunbody->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));
		gunbody->Set_Node_name("gunbody");

		gunslider = new modelNode(&Gunslider, &text[1]);//活塞模型节点地址
		gunslider->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(8,7, 5)));
		
		gunslider->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-20.0f, -6.0f, -8.0f)));
		gunslider->Set_Node_name("gunslider");
		
		for (int i=0;i<gunbulletcount;i++)
		{
         gunbullet[i] = new modelNode(&Gunbullet, &text[1]);//子弹节点地址
		 gunbullet[i]->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(7, 7, 7)));
		 gunbullet[i]->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 6.0f, -8.0f))//(o,o,o3)   o3
		 *glm::rotate(glm::mat4(1.0f),glm::radians(90.0f),glm::vec3(1,1,0)));
		
		  gunbullet[i]->Set_Node_name("gunbullet");
		}
         
		
		
		

		Root = new modelNode();
		Root->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)));
		Root->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 40.0f, 67.0f)) *glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0, 1, 0)));
		Root->Set_Node_name("Root");
		Root->Addchild(gunbody);
        Root->Addchild(gunslider);
		for (int i = 0; i < gunbulletcount; i++)
		{
			Root->Addchild(gunbullet[i]);
		}
	   
		
	

		Root0 = new modelNode();
		Root0->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(0.4, 0.4, 0.4)));
		Root0->Set_Transform(glm::mat4(1.0f));
		Root0->Addchild(Root);

	}
	void fire(float deltatime)
	{
		
		initGunNodeMatrix();
		
		gunslider->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(accumulation, -6.0f, -8.0f)));
		Root->Set_Transform(Root->GetNodetransform() * glm::rotate(glm::mat4(1.0f), glm::radians(-accumulation/4), glm::vec3(0, 0, 1)));
		Root->Set_Transform(Root->GetNodetransform() * glm::translate(glm::mat4(1.0f), glm::vec3(accumulation/3, 0.0f ,0.0f )));
		for (int i = 0; i < gunbulletcount; i++)
		{
             gunbullet[i]->Set_Transform(gunbullet[i]->GetNodetransform() * glm::translate(glm::mat4(1.0f),
				 glm::vec3(6.0f*bullet_accumulation *i,11.0f* bullet_accumulation -1.5f* bullet_accumulation*bullet_accumulation *i*0.7, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(bullet_accumulation * 20.0f), glm::vec3(1, 1, 1)));
		}
			
		accumulation += deltatime * 200.0f * direction;
		bullet_accumulation += deltatime * 20.0f ;
		if (accumulation > 40.0f)
		{
			direction = -1.0f;
			//bulletTooriginalposition();
		}
		else if (accumulation <-10.0f)
		{
			direction = 1.0f;
		}
		if (bullet_accumulation > 20.0f)
		{
			bullet_accumulation = 0.0f;
			
		}
		
	}

	modelNode* GetRootNode() { return Root0; }
private:
	mesh Gunbody;
	mesh Gunslider;
	mesh Gunbullet;

	modelNode* gunbody;
	modelNode* gunslider;
	modelNode* gunbullet[gunbulletcount];
	modelNode* Root;
	modelNode* Root0;
	texture2D text[3];

	float direction = 1.0f;
	float accumulation = 0.0f;
	float bullet_accumulation =0.0f;
};