#include "MDK_Anim.h"

MDK_Anim::MDK_Anim(const std::string& filename)
{
	if (filename.find(".md5anim") != std::string::npos)//֤�������ҵ�������ļ�
	{    
         std::ifstream  fin(filename,std::ios::in);
		 if (!fin)//���fin��������ֵ˵������û���ҵ���
		 {
			 std::cout << "cannot open this_file" << std::endl;
		 }
		 else//fin������ֵ �����ҵ�����
		 {
			 std::cout << "yes we find it let us load it" << std::endl;
		 }
	    int num_JOINTS = 0;
		int num_FRAMES = 0;
	    while (!fin.eof())
	    {
			std::string currentline;
			fin >> currentline;
			std::stringstream SS(currentline);//�ѵ�ǰ�е�������ת��Ϊ������
			std::string cmdline;
			SS >> cmdline;//���������뵽�ַ����� ��ֹ�е�if������ִ��
			if (cmdline=="numJoints")
			{
				fin>> numJoints;
				Joint = new MD5_ANIM_Joint[numJoints];//����һ���µĹ����ؽ�����
			}
			else if (cmdline=="numFrames")
			{
				fin >> num_frames;
				animated_frame = new MD5_frame[num_frames];//���ɶ�ӳһ��������֡���鲢�����֡�����Ԫ����float������
			}
			else if (cmdline == "numAnimatedComponents")
			{
				fin >> num_animate_components;
			}
			else if (cmdline == "frameRate")
			{
				fin >> framerate;
				std::cout << framerate << std::endl;
			}
			else if (cmdline=="frame")
			{
				LoadAnimateframe(fin,num_FRAMES);
			}
			else if (cmdline=="hierarchy")//�����˸��Ӽ̳й�ϵ
			{
				Loadhirearchy(fin, num_JOINTS);
				
			}
			else if (cmdline=="baseframe") {
				LoadAnimateBasepos(fin);				//Let's load in the base frame!
			}
        }
	}
	 

   
	
}
//���ظ��ӹ�ϵ
void MDK_Anim::Loadhirearchy(std::ifstream& from, int& count)
{
	std::string linebuffer;//������
	do
	{
		from >> linebuffer;
		if (linebuffer[0] == '"')
		{
			Joint[count].name = linebuffer.substr(1, linebuffer.find('"', 1) - 1);
			from>>Joint[count].parent_index;
			from >> Joint[count].component_flag;//ָʾ��άԪ��
			from >> Joint[count].start_index;//������ʼʱ������
			++count;
		}
	} while (linebuffer!="}");

}

void MDK_Anim::LoadAnimateframe(std::ifstream& from, int& count)
{

	int frame_Num;
	from >> frame_Num;
	animated_frame[frame_Num].component = new float[num_animate_components];//float�������С�����ַ
	std::string linebuffer;
	from >> linebuffer;
	if (linebuffer=="{")//��ʼ��
	{
		for (int i=0;i<num_animate_components;i++)
		{
			from>>animated_frame[frame_Num].component[i];
		}
	}
	count++;
}
//���س�ʼλ�� ��ת��
void MDK_Anim::LoadAnimateBasepos(std::ifstream& from)
{
	std::string linebuffer;
	char skipchar;
	int current_jointindex = 0;
	basepos.orientation = new glm::vec3[numJoints];
	basepos.position= new glm::vec3[numJoints];
	do
	{
		from >> linebuffer;
		if (linebuffer[0]=='(')
		{
			from >> basepos.position[current_jointindex].x;
			from >> basepos.position[current_jointindex].y; 
			from >> basepos.position[current_jointindex].z;

			from >> skipchar;
			from >> skipchar;


			from >> basepos.orientation[current_jointindex].x;
			from >> basepos.orientation[current_jointindex].y;
			from >> basepos.orientation[current_jointindex].z;
			from >> skipchar;
			current_jointindex++;
	    }
	} while (linebuffer!="}");
}
void MDK_Anim::transform_skeleton(MD5Skeleton& skeleton, int framenum)
{
	for (int Jointindex=0; Jointindex <numJoints; Jointindex++)
	{
		int j = 0;
		if (Joint[Jointindex].component_flag& MD5_ANIM_XPOS)//�����X����
		{
             basepos.position[Jointindex].x=animated_frame[framenum].component[Joint[Jointindex].start_index+j];
			 ++j;
		}
		if (Joint[Jointindex].component_flag & MD5_ANIM_YPOS)
		{
			basepos.position[Jointindex].y = animated_frame[framenum].component[Joint[Jointindex].start_index + j];
			++j;
		}
		if (Joint[Jointindex].component_flag & MD5_ANIM_ZPOS)
		{
			basepos.position[Jointindex].z = animated_frame[framenum].component[Joint[Jointindex].start_index + j];
			++j;
		}
		if (Joint[Jointindex].component_flag & MD5_ANIM_XQUAT)
		{
			basepos.orientation[Jointindex].x = animated_frame[framenum].component[Joint[Jointindex].start_index + j];
			++j;
		}
		if (Joint[Jointindex].component_flag & MD5_ANIM_YQUAT)
		{
			basepos.orientation[Jointindex].y = animated_frame[framenum].component[Joint[Jointindex].start_index + j];
			++j;
		}
		if (Joint[Jointindex].component_flag & MD5_ANIM_ZQUAT)
		{
			basepos.orientation[Jointindex].z= animated_frame[framenum].component[Joint[Jointindex].start_index + j];
			++j;
		}
		skeleton.joints[Jointindex].orientation = basepos.orientation[Jointindex];
		skeleton.joints[Jointindex].position = basepos.position[Jointindex];
		skeleton.joints[Jointindex].parent_index = Joint[Jointindex].parent_index;
		
		glm::mat4 model(1.0f);
		model = animationpos_toRotationMatrix(skeleton.joints[Jointindex].orientation);
		skeleton.joints[Jointindex].joint_transform = glm::translate(glm::mat4(1.0f), skeleton.joints[Jointindex].position)* model;
		skeleton.joints[Jointindex].initial_Joint_Matrix = skeleton.joints[Jointindex].joint_transform;
		if (skeleton.joints[Jointindex].parent_index<0)//��������Ǹ���
		{
			skeleton.joints[Jointindex].accumulate_transform = skeleton.joints[Jointindex].joint_transform;

		}
		else//�����Ǹ��� ������
		{
			
			skeleton.joints[Jointindex].accumulate_transform= 
			skeleton.joints[skeleton.joints[Jointindex].parent_index].accumulate_transform* skeleton.joints[Jointindex].joint_transform;//��������ٳ��ϸ����۳˾���
		}
	}
}

void MDK_Anim::generateW(glm::vec3& p)
{
	W = 1.0 - p.x * p.x - p.y * p.y - p.z * p.z;
	if (W<0.0f)
	{
		W = 0.0f;
	}
	else
	{
		W = -sqrt(W);
	}
}

glm::mat4 MDK_Anim::animationpos_toRotationMatrix(glm::vec3& p)
{
	generateW(p);
	glm::mat4 temp;
	temp[0][0] =1-2*p.y*p.y-2*p.z*p.z ;
	temp[0][1] = 2 * p.x * p.y - 2 * p.z * W;
	temp[0][2] = 2 * p.x * p.z + 2 * p.y * W;
	temp[0][3] = 0.0f;

	temp[1][0] = 2 * p.x * p.y +2 * p.z * W;
	temp[1][1] = 1 - 2 * p.x * p.x- 2 * p.z * p.z;
	temp[1][2] = 2 * p.y * p.z - 2 * p.x* W;
	temp[1][3] = 0.0f;

	temp[2][0] = 2 * p.x * p.z -2 * p.y * W;
	temp[2][1] = 2 * p.y * p.z + 2 * p.x * W;
	temp[2][2] = 1 - 2 * p.x * p.x - 2 * p.y * p.y;
	temp[2][3] = 0.0f;


	temp[3][0] = 0.0f;
	temp[3][1] = 0.0f;
	temp[3][2] = 0.0f;
	temp[3][3] =1.0f;

	return temp;
}
