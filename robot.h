#pragma once
#include"Texture2Dimage.h"
#include "mesh.h"
#include"model_Node.h"
#include"NodeDraw.h"
#include<map>
class robot:public Drawnode
{
public:
	void Robot_Create()
	{
		Body.loadobj("mesh\\body.obj");
		Head.loadobj("mesh\\monkey.obj");
		LeftArm.loadobj("mesh\\rightArm.obj");
		RightArm.loadobj("mesh\\rightArm.obj");
		Lefthand.loadobj("mesh\\right_hand.obj");
		Righthand.loadobj("mesh\\right_hand.obj");
		Leftleg.loadobj("mesh\\cube.obj");
		Rightleg.loadobj("mesh\\cube.obj");
		Fan.loadobj("mesh\\fan.obj");

		tex[0].loadtexture("teximage\\right.jpg");
		tex[1].loadtexture("teximage\\metal.jpg");
	}
	void Set_animationmap()
	{
		
		//animationmap[57][leftleg->Get_Node_name()] =
		//	glm::rotate(glm::mat4(1.0f), glm::radians(-2.80f), glm::vec3(1, 0, 0));
		//animationmap[58][leftleg->Get_Node_name()] =
		//	glm::rotate(glm::mat4(1.0f), glm::radians(-3.10f), glm::vec3(1, 0, 0));
		//animationmap[59][leftleg->Get_Node_name()] =
		//	glm::rotate(glm::mat4(1.0f), glm::radians(-4.70f), glm::vec3(1, 0, 0));
		//animationmap[60][leftleg->Get_Node_name()] =
		//	glm::rotate(glm::mat4(1.0f), glm::radians(-5.10f), glm::vec3(1, 0, 0));
		
		//ʱ��֡������������
		for (int i = 0; i < 78; i++)
		{
			animationmap[0][rightArm->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(0.06f), glm::vec3(0, 0, 1));
			animationmap[i][rightArm->Get_Node_name()] =
				 glm::rotate(glm::mat4(1.0f), glm::radians(i /1.06f), glm::vec3(0, 1, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(-84.06f), glm::vec3(0, 0, 1));//X Y Z
         
		}
		for (int i=78;i<175;i++)
		{
			animationmap[i][leftArm->Get_Node_name()]=
				glm::rotate(glm::mat4(1.0f), glm::radians(-i / 1.56f), glm::vec3(0, 1, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(84.06f), glm::vec3(0, 0, 1));//X Y Z
			animationmap[i][rightArm->Get_Node_name()] =
				glm::rotate(glm::mat4(1.0f), glm::radians(i / 5.06f), glm::vec3(1, 0, 0));
				//X Y Z
			animationmap[174][rightArm->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(0.06f), glm::vec3(0, 0, 1));
		}
		for (int i=175;i<260;i++)
		{
			animationmap[175][rightArm->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(0.06f), glm::vec3(0, 0, 1));
			animationmap[i][rightArm->Get_Node_name()] =
				glm::rotate(glm::mat4(1.0f), glm::radians((i-175) / 1.06f), glm::vec3(0, 1, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(-84.06f), glm::vec3(0, 0, 1));//X Y Z

			animationmap[i][leftArm->Get_Node_name()] =
				glm::rotate(glm::mat4(1.0f), glm::radians((i-78) / 1.56f), glm::vec3(1, 0, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(-175 / 1.56f), glm::vec3(1, 0, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(44.06f), glm::vec3(0, 0, 1));//X Y Z
			animationmap[258][leftArm->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(0.06f), glm::vec3(1, 0, 0));
		}
		for (int i=460;i<534;i++)
		{
			animationmap[i][rightArm->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(-34.06f), glm::vec3(1, 1, 1));
			animationmap[i][body->Get_Node_name()]= glm::rotate(glm::mat4(1.0f), glm::radians(i/0.73f), glm::vec3(1, 0, 1));
			animationmap[i][leftleg->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(-i / 1.06f), glm::vec3(1, 1, 1));
			//animationmap[358][body->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(0.00f), glm::vec3(1, 1, 1));
		}
		for (int i = 535; i < 600; i++)
		{
			animationmap[i][body->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1, 0, 1));
			animationmap[i][leftleg->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(-i / 6.06f), glm::vec3(1, 1, 1));
			animationmap[599][leftleg->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(0.06f), glm::vec3(1, 1, 1));
		}
		for (int i = 601; i < 700; i++)
		{
			animationmap[i][leftleg->Get_Node_name()] = glm::rotate(glm::mat4(1.0f), glm::radians(0.06f), glm::vec3(1, 1, 1));
		}
	}
	void initNodeMatrix()
	{  
		body = new modelNode(&Body, &tex[0]);//ģ�ͽڵ��ַ
		body->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2)));
		body->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f)));
		body->Set_bindPosetransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, 0.0f)));
		body->Set_Node_name("body");//�����Լ������� ƽ�ƾ���

		head = new modelNode(&Head, &tex[0]);//ģ�ͽڵ��ַ
		head->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)));
		head->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-2.3f, 0.3f, 2.3f)));
		head->Set_bindPosetransform(glm::translate(glm::mat4(1.0f), glm::vec3(-2.3f, 0.3f, 2.3f)));
		head->Set_Node_name("head");//�����Լ������� ƽ�ƾ���
		body->Addchild(head);

		fan = new modelNode(&Fan, &tex[1]);//ģ�ͽڵ��ַ
		fan->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)));
		fan->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 2.8f, -2.2f)));
		fan->Set_bindPosetransform(glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 2.8f, -2.2f)));
		fan->Set_Node_name("fan");//�����Լ������� ƽ�ƾ���
		head->Addchild(fan);

		leftArm = new modelNode(&LeftArm, &tex[0]);//ģ�ͽڵ��ַ
		leftArm->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)));
		leftArm->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.8f, 1.9f, 0.4f)) 
			* glm::rotate(glm::mat4(1.0f), glm::radians(22.0f), glm::vec3(0, 0, 1)));
		leftArm->Set_bindPosetransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.8f, 1.9f, 0.4f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(22.0f), glm::vec3(0, 0, 1)));
		leftArm->Set_Node_name("leftarm");//�����Լ������� ƽ�ƾ���
		body->Addchild(leftArm);

		lefthand = new modelNode(&Lefthand, &tex[0]);//ģ�ͽڵ��ַ
		lefthand->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(0.7, 0.7, 0.7)));
		lefthand->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.100001f, -2.845f, -0.853f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(152.0f), glm::vec3(1, 0, 1)));
		lefthand->Set_bindPosetransform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.100001f, -2.845f, -0.853f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(152.0f), glm::vec3(1, 0, 1)));
		lefthand->Set_Node_name("lefthand");//�����Լ������� ƽ�ƾ���
		leftArm->Addchild(lefthand);

		rightArm = new modelNode(&RightArm, &tex[0]);//ģ�ͽڵ��ַ
		rightArm->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)));
		rightArm->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.8f, 1.9f, 0.4f)) 
			* glm::rotate(glm::mat4(1.0f), glm::radians(-22.0f), glm::vec3(0, 0, 1)));
		rightArm->Set_bindPosetransform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.8f, 1.9f, 0.4f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(-22.0f), glm::vec3(0, 0, 1)));
		rightArm->Set_Node_name("righttarm");//�����Լ������� ƽ�ƾ���
		body->Addchild(rightArm);

		righthand = new modelNode(&Righthand, &tex[0]);//ģ�ͽڵ��ַ
		righthand->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.10001f, -2.845f, -0.853f)) 
			* glm::rotate(glm::mat4(1.0f), glm::radians(-152.0f), glm::vec3(1, 1, 1))
			* glm::rotate(glm::mat4(1.0f), glm::radians(-152.0f), glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(64.0f), glm::vec3(1, 1, 1)));
		righthand->Set_bindPosetransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.10001f, -2.845f, -0.853f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(-152.0f), glm::vec3(1, 1, 1))
			* glm::rotate(glm::mat4(1.0f), glm::radians(-152.0f), glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(64.0f), glm::vec3(1, 1, 1)));
		righthand->Set_Node_name("righthand");//�����Լ������� ƽ�ƾ���
		rightArm->Addchild(righthand);

		leftleg = new modelNode(&Leftleg, &tex[0]);
		leftleg->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 2, 0.1f)));//��С����
		leftleg->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, -0.3f, 0.27f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0)));//λ�ö���
		leftleg->Set_bindPosetransform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, -0.3f, 0.27f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0)));//λ�ö���
		leftleg->Set_Node_name("leftleg");//�����Լ������� ƽ�ƾ���
		body->Addchild(leftleg);


		rightleg = new modelNode(&Rightleg, &tex[0]);
		rightleg->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(0.2, 2, 0.1f)));//��С����
		rightleg->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, -0.3f, 0.27f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0)));//λ�ö���
		rightleg->Set_bindPosetransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, -0.3f, 0.27f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0)));//λ�ö���
		rightleg->Set_Node_name("rightleg");//�����Լ������� ƽ�ƾ���
		body->Addchild(rightleg);

		Root = new modelNode();
		Root->Set_modelscale(glm::scale(glm::mat4(1.0f), glm::vec3(10, 10, 10)));
		Root->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 86, 0)));
		Root->Set_bindPosetransform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 86, 0)));
		Root->Set_Node_name("Root");
		Root->Addchild(body);
	}
	void Run(float deltatime)
	{
		
		
		initNodeMatrix();

		leftArm->Set_Transform(leftArm->GetbindPosetransform() * glm::rotate(glm::mat4(1.0f), glm::radians(accumulation), glm::vec3(1, 0, 0)));
		fan->Set_Transform(fan->GetbindPosetransform() * glm::rotate(glm::mat4(1.0f), glm::radians(fan_accumulation ), glm::vec3(0, 1, 0)));
		rightArm->Set_Transform(rightArm->GetbindPosetransform() * glm::rotate(glm::mat4(1.0f), glm::radians(-accumulation), glm::vec3(1, 0, 0)));
		leftleg->Set_Transform(leftleg->GetbindPosetransform() * glm::rotate(glm::mat4(1.0f), glm::radians(accumulation), glm::vec3(1, 0, 0)));
		rightleg->Set_Transform(rightleg->GetbindPosetransform() * glm::rotate(glm::mat4(1.0f), glm::radians(-accumulation), glm::vec3(1, 0, 0)));
		accumulation += deltatime * 200.0f * direction;
		fan_accumulation+= deltatime * 200.0f ;
		if (accumulation > 70.0f)
		{
			direction = -1.0f;
		}
		else if (accumulation < -70.0f)
		{
			direction = 1.0f;
		}
	}
	void punch(float deltatime)
	{
		initNodeMatrix();

		rightArm->Set_Transform(rightArm->GetbindPosetransform() * glm::rotate(glm::mat4(1.0f), glm::radians(-accumulation), glm::vec3(1, 1, 0)));
		rightleg->Set_Transform(rightleg->GetbindPosetransform() * glm::rotate(glm::mat4(1.0f), glm::radians(-accumulation), glm::vec3(1, 0, 1)));
		accumulation += deltatime * 200.0f * direction;
		if (accumulation > 170.0f)
		{
			direction = -1.0f;
		}
		else if (accumulation < 0.0f)
		{
			direction = 1.0f;
		}
	}
	void flip(float deltatime)
	{
		initNodeMatrix();
		rightArm->Set_Transform(rightArm->GetbindPosetransform() * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0)));
		leftArm->Set_Transform(glm::translate(glm::mat4(1.0f), glm::vec3(0.8f, 1.9f, 0.4f)) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0)));
		leftleg->Set_Transform(leftleg->GetbindPosetransform() *glm::rotate(glm::mat4(1.0f),glm::radians(49.0f),glm::vec3(1,0,0)));
		body->Set_Transform(body->GetbindPosetransform() * glm::rotate(glm::mat4(1.0f), glm::radians(-accumulation), glm::vec3(1, 0, 0)));
		
		accumulation += deltatime * 200.0f * direction;
		
	}
	void animationPlay(modelNode*Node,int animation_Time_index,shaderProgram*shader)//������������
	{
		//initNodeMatrix();
		
		if (animationmap[animation_Time_index].find(Node->Get_Node_name()) != animationmap[animation_Time_index].end())
		{
			Node->Set_Transform(Node->GetbindPosetransform() * animationmap[animation_Time_index][Node->Get_Node_name()]);
			  DrawNode(Node,shader);
		}//�Ƿ���Դ����ʱ��֡�ҵ���������ڶ� �����������˶�������˵�����������û�������ʱ����ڶ�
	
		
		for (std::vector < modelNode* >::const_iterator i = Node->Getchildbegin(); i != Node->Getchildend(); ++i)
		{
			animationPlay(*i,animation_Time_index,shader);
		}//һ��������нڵ�任����Ϳ�ʼ������
		

	}
	modelNode* GetRootNode() { return Root; }
private:
	texture2D tex[3];

	

	mesh Body;
	mesh Head;
	mesh LeftArm;
	mesh RightArm;
	mesh Lefthand;
	mesh Righthand;
	mesh Leftleg;
	mesh Rightleg;
	mesh Fan;

	modelNode* Root;
	modelNode* body;
	modelNode* head;
	modelNode* leftArm;
	modelNode* rightArm;
	modelNode* leftleg;
	modelNode* rightleg;
	modelNode* fan;
	modelNode* lefthand;
	modelNode* righthand;

	
	float direction = 1.0f;
	float accumulation = 0.0f;
	float fan_accumulation = 0.0f;

	
	std::map<int, std::map<const char*, glm::mat4>>animationmap;//��������ʱ��������ڵ���ƥ�� ����ĳһ���ض�ʱ�������Щ�����ڶ� ��Щ����
};
////void animation_draw(Time){
//
//animationmap[Time].find("head")==true;
//head->set_transform(animationmap[Time]["head"]);
//animationmap[Time].find("leftleg")==true;
//leftleg->set_transform(animationmap[Time]["leftleg"]);
 //animationmap[Time].find("rightleg")==true;
//rightleg->set_transform(animationmap[Time]["rightleg"])
 //animationmap[Time].find("lefthand")==true;
//lefthand->set_transform(animationmap[Time]["lefthand"]);
//...........
//Root->update(angle);//���˽ڵ����
// }
