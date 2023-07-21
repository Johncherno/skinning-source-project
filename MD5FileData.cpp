#include "MD5FileData.h"
#include<iostream>


MD5FileData::MD5FileData(const std::string& filename)
{
	if (filename.find(".md5mesh") != std::string::npos)//֤�������ҵ�������ļ�
	{
		std::ifstream fin(filename, std::ios::in);//�ļ����������� 
		if (!fin)//���fin��������ֵ˵������û���ҵ���
		{
			std::cout << "cannot open this md5mesh_file" << std::endl;
		}
		else//fin������ֵ �����ҵ�����
		{
			std::cout << "yes we find it let us load it" << std::endl;
		}
		 int num_mesh_loaded=0;
		 int num_joint_loaded = 0;
		 int numExpectedjoints;
		 int numExpectedMeshes;	
		while (!fin.eof())//����ĵ�û��ͷ
		{
		    std::string currentline;
			fin >> currentline;//�ĵ��ַ������뵽��ǰ�����ַ����еı�����

			std::stringstream SS(currentline);//�ѵ�ǰ�е�������ת��Ϊ������
			std::string cmdline;
			SS >> cmdline;//���������뵽�ַ����� ��ֹ�е�if������ִ��
			if (cmdline.empty())
			{
				continue;
			}
			else if (cmdline == "numJoints")
			{
				fin >> numExpectedjoints;
				joint = new MD5joint[numExpectedjoints];
				std::cout << "�ؽڽڵ���" << numExpectedjoints << std::endl;
			}
			
			else if (cmdline == "numMeshes") {
				fin >> numExpectedMeshes; //load in the number of submeshes held in this md5mesh
				std::cout << "��������" << numExpectedMeshes << std::endl;
				submesh = new MD5subMesh[numExpectedMeshes];
				
			}
			
			else if (cmdline == "joints")//�ؽ�
			{
				num_joint_loaded+=LoadMD5Joints(fin);
				std::cout << "we are loading joints" << std::endl;
			}
			else if (cmdline =="mesh")//����
			{   std::cout << "�����ҵ���mesh �ڼ�������" <<num_mesh_loaded << std::endl;
			   loadMesh(fin, num_mesh_loaded);
			   ++num_mesh_loaded;
			   std::cout << "   " << std::endl;
			}
			
		}

		num_sub_meshes = num_mesh_loaded;
		num_joints = num_joint_loaded;
		std::cout << "һ�������˶��ٸ�������" << num_mesh_loaded << std::endl;
		std::cout << "һ�������˶��ٸ��ӹ���" << num_joint_loaded << std::endl;
	}
     CreateMesh();
	 CreateTBO();
}
void MD5FileData::clone_skeletoninformation (MD5Skeleton& destination_skeleton)const
{

	destination_skeleton.numJoints = this->num_joints;
	destination_skeleton.joints = new MD5joint[this->num_joints];//��ֹ���ʳ�ͻ
	memcpy((void*)destination_skeleton.joints,(void*)joint,sizeof(MD5joint)*this->num_joints);//��ÿ�������ڵ���Ϣ���̳�ȥ

}


glm::mat4 MD5FileData::toRotation(glm::vec3&p)
{
	glm::mat4 temp;
	temp[0][0] = 1 - 2 * p.y * p.y - 2 * p.z * p.z;
	temp[0][1] = 2 * p.x * p.y - 2 * p.z * 1.0f;
	temp[0][2] = 2 * p.x * p.z + 2 * p.y * 1.0f;
	temp[0][3] = 0.0f;

	temp[1][0] = 2 * p.x * p.y + 2 * p.z * 1.0f;
	temp[1][1] = 1 - 2 * p.x * p.x - 2 * p.z * p.z;
	temp[1][2] = 2 * p.y * p.z - 2 * p.x * 1.0f;
	temp[1][3] = 0.0f;

	temp[2][0] = 2 * p.x * p.z - 2 * p.y * 1.0f;
	temp[2][1] = 2 * p.y * p.z + 2 * p.x * 1.0f;
	temp[2][2] = 1 - 2 * p.x * p.x - 2 * p.y * p.y;
	temp[2][3] = 0.0f;


	temp[3][0] = 0.0f;
	temp[3][1] = 0.0f;
	temp[3][2] = 0.0f;
	temp[3][3] = 1.0f;

	return temp;
}


void MD5FileData::loadMesh(std::ifstream& from, int& Meshcount)
{
	 int verts_loaded = 0;
	 int triangular_faces_loaded = 0;
	 int weights_loaded = 0;
	 char skipchar;//�����������������
	 std::cout << "�ڼ�������" << Meshcount << std::endl;
	//MD5subMesh& md5mesh = submesh[Meshcount];//���ٸ�����Ƥ����  ��ȡ����ĵ�ַ
	std::string linebuffer;
	do {
		
		
		from >> linebuffer;//���ȥ����ַ�����
		if (linebuffer == "shader")
		{
			std::string shadername;
			from >> shadername;
			std::cout << shadername << std::endl;
		}//���������ɫ������
		else if (linebuffer == "numverts")
		{
		
			from >> submesh[Meshcount].numverts;
			submesh[Meshcount].vert = new MD5vert[submesh[Meshcount].numverts];//newһ���µĶ�������
			std::cout << "���ٸ���������" << submesh[Meshcount].numverts << std::endl;
		}
		else if (linebuffer == "numtris")
		{
			from >> submesh[Meshcount].numtriangulatefaces;
			submesh[Meshcount].triangular_face = new MD5triangulte_face[submesh[Meshcount].numtriangulatefaces];
			std::cout << "���ٸ�������������" << submesh[Meshcount].numtriangulatefaces << std::endl;
		}
		else if (linebuffer == "numweights")
		{
			from >> submesh[Meshcount].numweights;
			submesh[Meshcount].weight = new MD5weight[submesh[Meshcount].numweights];
			std::cout << "���ٸ�Ȩ�ؽṹ������" << submesh[Meshcount].numweights << std::endl;
		}
		else if (linebuffer=="vert")//����
		{
			from >> submesh[Meshcount].vert[verts_loaded].vert_index;//���붥������
			from >> skipchar;
			from >> submesh[Meshcount].vert[verts_loaded].Texcoord.x;
			from >> submesh[Meshcount].vert[verts_loaded].Texcoord.y;//��������
			from >> skipchar;
			from >> submesh[Meshcount].vert[verts_loaded].first_weightindex;//Ӱ���������ĵ�һ�������ڵ�İٷ���ռ��
			from >> submesh[Meshcount].vert[verts_loaded].num_weights;//�ӵ�һ�������ڵ㿪ʼ���𵽵��ж��ٸ������ڵ���Ӱ���� Ȩ��
			verts_loaded++;//������һ�ε�ѭ��
		}
		else if (linebuffer=="tri")//��������
		{
			from >> submesh[Meshcount].triangular_face[triangular_faces_loaded].triangulte_face_index;//�������������
			from >> submesh[Meshcount].triangular_face[triangular_faces_loaded].first_vertice_index;//��һ�������ζ���
			from >> submesh[Meshcount].triangular_face[triangular_faces_loaded].second_vertice_index;//�ڶ��������ζ���
			from >> submesh[Meshcount].triangular_face[triangular_faces_loaded].third_vertice_index;//�����������ζ���
			triangular_faces_loaded++; //������һ�ε�ѭ��
		}
		else if (linebuffer == "weight")//Ȩ��ֵ ��������
		{
			from >>submesh[Meshcount].weight[weights_loaded].weight_index;
			from >>submesh[Meshcount].weight[weights_loaded].joint_index;//��������
			from >>submesh[Meshcount].weight[weights_loaded].weight_value;//����������������ڵ��Ȩ��ֵ
			from >> skipchar;
			from >> submesh[Meshcount].weight[weights_loaded].position.x;//λ������
			from >> submesh[Meshcount].weight[weights_loaded].position.y;
			from >> submesh[Meshcount].weight[weights_loaded].position.z;
			from >> skipchar;//��������
			
			weights_loaded++;
		}
	} while (linebuffer!= "}");//��û�е�������������ʱ�ĵ���û���Ķ�����
}
int MD5FileData::LoadMD5Joints(std::ifstream& from)
{
	int joint_loaded = 0;//�����ڵ�ļ����˶���
	char skipchar;//��������
	std::string templine;
	do
	{
		from >> templine;
		
		if (templine[0] == '"')//������˫����
		{
			
			//����ؽڽڵ�����
			joint[joint_loaded].name = templine.substr(1, templine.find('"', 1) - 1);//�ӵ�һ���ַ���˫����ǰ����ַ�    "  body2 
		
			from >> joint[joint_loaded].parent_index;
	
			from >> skipchar;
			from >> joint[joint_loaded].position.x;
			from >> joint[joint_loaded].position.y;
			from >> joint[joint_loaded].position.z;
			from >> skipchar;
	
			
	
			from >> skipchar;
			from >> joint[joint_loaded].orientation.x;
			from >> joint[joint_loaded].orientation.y;
			from >> joint[joint_loaded].orientation.z;
			from >> skipchar;
	
		
		
			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::rotate(model, joint[joint_loaded].orientation.x, glm::vec3(1, 0, 0));//��X��ŷ���Ƕ���ת
			//model = glm::rotate(model, joint[joint_loaded].orientation.y, glm::vec3(0, 1, 0));//��Y��ŷ���Ƕ���ת
			//model = glm::rotate(model, joint[joint_loaded].orientation.z, glm::vec3(0, 0, 1));//��Z��ŷ���Ƕ���ת
			model = toRotation(joint[joint_loaded].orientation);
			joint[joint_loaded].joint_transform = glm::translate(glm::mat4(1.0f), joint[joint_loaded].position) * model;//ƽ�ƾ���*��ת����
			++joint_loaded;//������ƫ�Ʒ�����һ��ѭ��������������
		}
	} while (templine != "}");

	return joint_loaded;
}
void MD5FileData::CreateTBO()
{//Ȩ�ػ���
	glGenBuffers(1, &weightbuffer);
	//���󻺳�
	glGenBuffers(1, &transformbuffer);

	glGenTextures(1, &weightTexture);
	glGenTextures(1, &transformTexture);

	int weightcount = 0;
	for (int i = 0; i < num_sub_meshes; i++)
	{
		weightcount += submesh[i].numweights;
	}//�ܼ����е������Ȩ������
	weighting = new glm::vec3[weightcount * 2];//�����weighting��ַ  �ֳ�����
	Transform = new glm::mat4[num_joints ];//һ�����������ؽڽڵ���� һ��������������
	for (int i=0;i<num_joints;i++)
	{
		Transform[i] = joint[i].joint_transform;
	}
	int currentweightindex = 0;
	for (int i = 0; i < num_sub_meshes; i++)
	{
		for (int j = 0; j < submesh[i].numweights; j++)
		{
			weighting[currentweightindex * 2 + 0] = glm::vec3(
				submesh[i].weight[j].weight_index,
				submesh[i].weight[j].joint_index,
				submesh[i].weight[j].weight_value
			);
			weighting[currentweightindex * 2 + 1] = submesh[i].weight[j].position;
			currentweightindex++;//�ܼ����������Ȩ������
		}
	}
	
	glBindBuffer(GL_TEXTURE_BUFFER, weightbuffer);
	glBufferData(GL_TEXTURE_BUFFER, weightcount * sizeof(glm::vec3) *2, &weighting[0], GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	
	//��������transform�Ͳ��󶨵�OpenGL�� ��ΪҪ����ʱ�����ı�
	
	glBindBuffer(GL_TEXTURE_BUFFER, transformbuffer);
	glBufferData(GL_TEXTURE_BUFFER, num_joints * sizeof(glm::mat4), &Transform[0], GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
}
void MD5FileData::BindTextureBuffers()const
{  
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_BUFFER, weightTexture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, weightbuffer);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_BUFFER, transformTexture);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, transformbuffer);
}

void MD5FileData::update_transformTBO(MD5Skeleton&skeleton)const
{
	
	for (int jointindex=0;jointindex<num_joints;jointindex++)
	{
		
		Transform[jointindex] = skeleton.joints[jointindex].accumulate_transform;
	   
		
	}
	glBindBuffer(GL_TEXTURE_BUFFER, transformbuffer);
	glBufferSubData(GL_TEXTURE_BUFFER, 0,num_joints * sizeof(glm::mat4), (void*)&Transform[0]);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

}

void MD5FileData::CreateMesh()
{    
	 md5mesh = new MD5mesh[num_sub_meshes];//newһ��������
	 int indexstart = 0;
	for (int i=0;i<num_sub_meshes;i++)//������������
	{
		md5mesh[i].num_vertices = submesh[i].numverts;
		md5mesh[i].num_indices = submesh[i].numtriangulatefaces*3;
		//��mdk5��������ָ������ַ
		md5mesh[i].indices = new int[md5mesh[i].num_indices];
		md5mesh[i].skeletonVert = new SkeletonVertex[md5mesh[i].num_vertices];
		//std::cout << "mdk5" << "��" << i << "�� " <<"����Ķ�������"<< md5mesh[i].num_vertices << std::endl;
		//std::cout << "mdk5" << "��" << i << "�� " <<"����Ķ�����������"<< md5mesh[i].num_indices << std::endl;
       //������������Ķ�������ֵ
		for (int j=0;j< submesh[i].numtriangulatefaces;++j)
		{
			md5mesh[i].indices[(j * 3) + 0] = submesh[i].triangular_face[j].third_vertice_index;//��������ڼ�����ĵ���������
			md5mesh[i].indices[(j * 3) + 1] = submesh[i].triangular_face[j].second_vertice_index;
			md5mesh[i].indices[(j * 3) + 2] = submesh[i].triangular_face[j].first_vertice_index;
		}
		
		//��������
		//Also Buffer this submeshes 'extra' data (its weightings)
		for (int j = 0; j < submesh[i].numverts; j++)//��������
		{ 
			md5mesh[i].skeletonVert[j].Texcoord.x = submesh[i].vert[j].Texcoord.x;
			md5mesh[i].skeletonVert[j].Texcoord.y = submesh[i].vert[j].Texcoord.y;

			glm::vec4 temp = glm::vec4(0, 0, 0, 1);
			for (int m = 0; m < submesh[i].vert[j].num_weights; m++)
			{//��������
				md5mesh[i].skeletonVert[j].Texcoord.x = submesh[i].vert[j].Texcoord.x;
				md5mesh[i].skeletonVert[j].Texcoord.y = submesh[i].vert[j].Texcoord.y;
				MD5weight& WEIGHT = submesh[i].weight[submesh[i].vert[j].first_weightindex + m];
				temp += joint[WEIGHT.joint_index].joint_transform * WEIGHT.weight_value * glm::vec4(WEIGHT.position, 1.0f);
			}
			md5mesh[i].skeletonVert[j].Vertice = vec4_to_vec3(temp);//���ɶ�����ڸ������ɷ���

		 md5mesh[i].skeletonVert[j].WEighting.x = (float)submesh[i].vert[j].num_weights;
		 md5mesh[i].skeletonVert[j].WEighting.y = (float)submesh[i].vert[j].first_weightindex + indexstart;
		
		}//vertices
		//  |
		//  |
		//  |
		//  |
		//  |
		//  |
		//  |
		//  |
		// \/
		// normal
		//����
		for (int p = 0; p < submesh[i].numtriangulatefaces; p++)//���ٸ���
		{
			int third = submesh[i].triangular_face[p].third_vertice_index;
			int second = submesh[i].triangular_face[p].second_vertice_index;
			int first = submesh[i].triangular_face[p].first_vertice_index;

			glm::vec3 NORMAL = glm::cross(md5mesh[i].skeletonVert[third].Vertice - md5mesh[i].skeletonVert[first].Vertice,
				md5mesh[i].skeletonVert[third].Vertice - md5mesh[i].skeletonVert[second].Vertice);
			NORMAL = glm::normalize(NORMAL);
			md5mesh[i].skeletonVert[third].Normal = NORMAL;
			md5mesh[i].skeletonVert[second].Normal = NORMAL;
			md5mesh[i].skeletonVert[first].Normal = NORMAL;

		}
		 indexstart += submesh[i].numweights;
		md5mesh[i].Create_VAO_VBO_EBO_Buffer_Object();//�ȶ�ÿһ��������󴴽��ø��ԵĻ���
		md5mesh[i].Bind_PerskeletonVertice_To_GPU();//��OpenGL�Ķ�������
	}
	
}
void MD5FileData::Draw()const
{
	
	for (int i = 0; i < num_sub_meshes; i++)//������������
	{
        md5mesh[i].Draw();
	}
	
}
glm::vec3 MD5FileData::vec4_to_vec3(glm::vec4& temp)
{           glm::vec3 tempp;
			tempp.x =temp.x/temp.w;
			tempp.y= temp.y / temp.w;
			tempp.z = temp.z / temp.w;
			return tempp;
}

