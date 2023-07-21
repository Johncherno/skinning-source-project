#include "MDK_Anim.h"

MDK_Anim::MDK_Anim(const std::string& filename)
{
	if (filename.find(".md5anim") != std::string::npos)//证明我们找到了这个文件
	{    
         std::ifstream  fin(filename,std::ios::in);
		 if (!fin)//如果fin不等于真值说明我们没有找到它
		 {
			 std::cout << "cannot open this_file" << std::endl;
		 }
		 else//fin等于真值 我们找到了它
		 {
			 std::cout << "yes we find it let us load it" << std::endl;
		 }
	    int num_JOINTS = 0;
		int num_FRAMES = 0;
	    while (!fin.eof())
	    {
			std::string currentline;
			fin >> currentline;
			std::stringstream SS(currentline);//把当前行的数据行转化为数据流
			std::string cmdline;
			SS >> cmdline;//数据流输入到字符串中 防止有的if条件不执行
			if (cmdline=="numJoints")
			{
				fin>> numJoints;
				Joint = new MD5_ANIM_Joint[numJoints];//生成一个新的骨骼关节数组
			}
			else if (cmdline=="numFrames")
			{
				fin >> num_frames;
				animated_frame = new MD5_frame[num_frames];//生成对映一定数量的帧数组并且这个帧数组的元素是float的数组
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
			else if (cmdline=="hierarchy")//遇到了父子继承关系
			{
				Loadhirearchy(fin, num_JOINTS);
				
			}
			else if (cmdline=="baseframe") {
				LoadAnimateBasepos(fin);				//Let's load in the base frame!
			}
        }
	}
	 

   
	
}
//加载父子关系
void MDK_Anim::Loadhirearchy(std::ifstream& from, int& count)
{
	std::string linebuffer;//数据流
	do
	{
		from >> linebuffer;
		if (linebuffer[0] == '"')
		{
			Joint[count].name = linebuffer.substr(1, linebuffer.find('"', 1) - 1);
			from>>Joint[count].parent_index;
			from >> Joint[count].component_flag;//指示三维元素
			from >> Joint[count].start_index;//动画开始时间索引
			++count;
		}
	} while (linebuffer!="}");

}

void MDK_Anim::LoadAnimateframe(std::ifstream& from, int& count)
{

	int frame_Num;
	from >> frame_Num;
	animated_frame[frame_Num].component = new float[num_animate_components];//float的数组大小分配地址
	std::string linebuffer;
	from >> linebuffer;
	if (linebuffer=="{")//开始处
	{
		for (int i=0;i<num_animate_components;i++)
		{
			from>>animated_frame[frame_Num].component[i];
		}
	}
	count++;
}
//加载初始位置 旋转角
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
		if (Joint[Jointindex].component_flag& MD5_ANIM_XPOS)//如果有X分量
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
		if (skeleton.joints[Jointindex].parent_index<0)//它本身就是父类
		{
			skeleton.joints[Jointindex].accumulate_transform = skeleton.joints[Jointindex].joint_transform;

		}
		else//它不是父类 是子类
		{
			
			skeleton.joints[Jointindex].accumulate_transform= 
			skeleton.joints[skeleton.joints[Jointindex].parent_index].accumulate_transform* skeleton.joints[Jointindex].joint_transform;//子类矩阵再乘上父类累乘矩阵
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
