#include "MD5FileData.h"
#include<iostream>


MD5FileData::MD5FileData(const std::string& filename)
{
	if (filename.find(".md5mesh") != std::string::npos)//证明我们找到了这个文件
	{
		std::ifstream fin(filename, std::ios::in);//文件数据输入流 
		if (!fin)//如果fin不等于真值说明我们没有找到它
		{
			std::cout << "cannot open this md5mesh_file" << std::endl;
		}
		else//fin等于真值 我们找到了它
		{
			std::cout << "yes we find it let us load it" << std::endl;
		}
		 int num_mesh_loaded=0;
		 int num_joint_loaded = 0;
		 int numExpectedjoints;
		 int numExpectedMeshes;	
		while (!fin.eof())//如果文档没到头
		{
		    std::string currentline;
			fin >> currentline;//文档字符流输入到当前数据字符串行的变量里

			std::stringstream SS(currentline);//把当前行的数据行转化为数据流
			std::string cmdline;
			SS >> cmdline;//数据流输入到字符串中 防止有的if条件不执行
			if (cmdline.empty())
			{
				continue;
			}
			else if (cmdline == "numJoints")
			{
				fin >> numExpectedjoints;
				joint = new MD5joint[numExpectedjoints];
				std::cout << "关节节点数" << numExpectedjoints << std::endl;
			}
			
			else if (cmdline == "numMeshes") {
				fin >> numExpectedMeshes; //load in the number of submeshes held in this md5mesh
				std::cout << "骨骼数量" << numExpectedMeshes << std::endl;
				submesh = new MD5subMesh[numExpectedMeshes];
				
			}
			
			else if (cmdline == "joints")//关节
			{
				num_joint_loaded+=LoadMD5Joints(fin);
				std::cout << "we are loading joints" << std::endl;
			}
			else if (cmdline =="mesh")//网格
			{   std::cout << "我们找到了mesh 第几个网格" <<num_mesh_loaded << std::endl;
			   loadMesh(fin, num_mesh_loaded);
			   ++num_mesh_loaded;
			   std::cout << "   " << std::endl;
			}
			
		}

		num_sub_meshes = num_mesh_loaded;
		num_joints = num_joint_loaded;
		std::cout << "一共加载了多少个子网格" << num_mesh_loaded << std::endl;
		std::cout << "一共加载了多少个子骨骼" << num_joint_loaded << std::endl;
	}
     CreateMesh();
	 CreateTBO();
}
void MD5FileData::clone_skeletoninformation (MD5Skeleton& destination_skeleton)const
{

	destination_skeleton.numJoints = this->num_joints;
	destination_skeleton.joints = new MD5joint[this->num_joints];//防止访问冲突
	memcpy((void*)destination_skeleton.joints,(void*)joint,sizeof(MD5joint)*this->num_joints);//将每个骨骼节点信息复刻出去

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
	 char skipchar;//用来跳出坐标的括号
	 std::cout << "第几个网格" << Meshcount << std::endl;
	//MD5subMesh& md5mesh = submesh[Meshcount];//多少个子蒙皮网格  获取这个的地址
	std::string linebuffer;
	do {
		
		
		from >> linebuffer;//输进去这个字符串流
		if (linebuffer == "shader")
		{
			std::string shadername;
			from >> shadername;
			std::cout << shadername << std::endl;
		}//如果碰到着色器名字
		else if (linebuffer == "numverts")
		{
		
			from >> submesh[Meshcount].numverts;
			submesh[Meshcount].vert = new MD5vert[submesh[Meshcount].numverts];//new一个新的顶点数组
			std::cout << "多少个顶点数量" << submesh[Meshcount].numverts << std::endl;
		}
		else if (linebuffer == "numtris")
		{
			from >> submesh[Meshcount].numtriangulatefaces;
			submesh[Meshcount].triangular_face = new MD5triangulte_face[submesh[Meshcount].numtriangulatefaces];
			std::cout << "多少个三角形面数量" << submesh[Meshcount].numtriangulatefaces << std::endl;
		}
		else if (linebuffer == "numweights")
		{
			from >> submesh[Meshcount].numweights;
			submesh[Meshcount].weight = new MD5weight[submesh[Meshcount].numweights];
			std::cout << "多少个权重结构体数量" << submesh[Meshcount].numweights << std::endl;
		}
		else if (linebuffer=="vert")//顶点
		{
			from >> submesh[Meshcount].vert[verts_loaded].vert_index;//输入顶点索引
			from >> skipchar;
			from >> submesh[Meshcount].vert[verts_loaded].Texcoord.x;
			from >> submesh[Meshcount].vert[verts_loaded].Texcoord.y;//纹理坐标
			from >> skipchar;
			from >> submesh[Meshcount].vert[verts_loaded].first_weightindex;//影响这个顶点的第一个骨骼节点的百分数占比
			from >> submesh[Meshcount].vert[verts_loaded].num_weights;//从第一个骨骼节点开始算起到底有多少个骨骼节点在影响它 权重
			verts_loaded++;//便于下一次的循环
		}
		else if (linebuffer=="tri")//三角形面
		{
			from >> submesh[Meshcount].triangular_face[triangular_faces_loaded].triangulte_face_index;//三角形面的索引
			from >> submesh[Meshcount].triangular_face[triangular_faces_loaded].first_vertice_index;//第一个三角形顶点
			from >> submesh[Meshcount].triangular_face[triangular_faces_loaded].second_vertice_index;//第二个三角形顶点
			from >> submesh[Meshcount].triangular_face[triangular_faces_loaded].third_vertice_index;//第三个三角形顶点
			triangular_faces_loaded++; //便于下一次的循环
		}
		else if (linebuffer == "weight")//权重值 骨骼索引
		{
			from >>submesh[Meshcount].weight[weights_loaded].weight_index;
			from >>submesh[Meshcount].weight[weights_loaded].joint_index;//骨骼索引
			from >>submesh[Meshcount].weight[weights_loaded].weight_value;//顶点绕着这个骨骼节点的权重值
			from >> skipchar;
			from >> submesh[Meshcount].weight[weights_loaded].position.x;//位置坐标
			from >> submesh[Meshcount].weight[weights_loaded].position.y;
			from >> submesh[Meshcount].weight[weights_loaded].position.z;
			from >> skipchar;//跳出括号
			
			weights_loaded++;
		}
	} while (linebuffer!= "}");//当没有到达最后的下括号时文档还没有阅读结束
}
int MD5FileData::LoadMD5Joints(std::ifstream& from)
{
	int joint_loaded = 0;//骨骼节点的加载了多少
	char skipchar;//跳出括号
	std::string templine;
	do
	{
		from >> templine;
		
		if (templine[0] == '"')//遇到了双引号
		{
			
			//放入关节节点名字
			joint[joint_loaded].name = templine.substr(1, templine.find('"', 1) - 1);//从第一个字符到双引号前面的字符    "  body2 
		
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
			//model = glm::rotate(model, joint[joint_loaded].orientation.x, glm::vec3(1, 0, 0));//先X轴欧拉角度旋转
			//model = glm::rotate(model, joint[joint_loaded].orientation.y, glm::vec3(0, 1, 0));//再Y轴欧拉角度旋转
			//model = glm::rotate(model, joint[joint_loaded].orientation.z, glm::vec3(0, 0, 1));//再Z轴欧拉角度旋转
			model = toRotation(joint[joint_loaded].orientation);
			joint[joint_loaded].joint_transform = glm::translate(glm::mat4(1.0f), joint[joint_loaded].position) * model;//平移矩阵*旋转矩阵
			++joint_loaded;//再往下偏移方便下一次循环遍历骨骼索引
		}
	} while (templine != "}");

	return joint_loaded;
}
void MD5FileData::CreateTBO()
{//权重缓冲
	glGenBuffers(1, &weightbuffer);
	//矩阵缓冲
	glGenBuffers(1, &transformbuffer);

	glGenTextures(1, &weightTexture);
	glGenTextures(1, &transformTexture);

	int weightcount = 0;
	for (int i = 0; i < num_sub_meshes; i++)
	{
		weightcount += submesh[i].numweights;
	}//总计所有的网格的权重数量
	weighting = new glm::vec3[weightcount * 2];//分配给weighting地址  分成两列
	Transform = new glm::mat4[num_joints ];//一个用来骨骼关节节点计算 一个用来调整法线
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
			currentweightindex++;//总计所有网格的权重数量
		}
	}
	
	glBindBuffer(GL_TEXTURE_BUFFER, weightbuffer);
	glBufferData(GL_TEXTURE_BUFFER, weightcount * sizeof(glm::vec3) *2, &weighting[0], GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	
	//接下来的transform就不绑定到OpenGL了 因为要随着时间来改变
	
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
	 md5mesh = new MD5mesh[num_sub_meshes];//new一堆子网格
	 int indexstart = 0;
	for (int i=0;i<num_sub_meshes;i++)//遍历所有网格
	{
		md5mesh[i].num_vertices = submesh[i].numverts;
		md5mesh[i].num_indices = submesh[i].numtriangulatefaces*3;
		//给mdk5网格数组指针分配地址
		md5mesh[i].indices = new int[md5mesh[i].num_indices];
		md5mesh[i].skeletonVert = new SkeletonVertex[md5mesh[i].num_vertices];
		//std::cout << "mdk5" << "第" << i << "个 " <<"网格的顶点数量"<< md5mesh[i].num_vertices << std::endl;
		//std::cout << "mdk5" << "第" << i << "个 " <<"网格的顶点索引数量"<< md5mesh[i].num_indices << std::endl;
       //遍历单个网格的顶点索引值
		for (int j=0;j< submesh[i].numtriangulatefaces;++j)
		{
			md5mesh[i].indices[(j * 3) + 0] = submesh[i].triangular_face[j].third_vertice_index;//单个网格第几个面的第三个顶点
			md5mesh[i].indices[(j * 3) + 1] = submesh[i].triangular_face[j].second_vertice_index;
			md5mesh[i].indices[(j * 3) + 2] = submesh[i].triangular_face[j].first_vertice_index;
		}
		
		//顶点索引
		//Also Buffer this submeshes 'extra' data (its weightings)
		for (int j = 0; j < submesh[i].numverts; j++)//单个网格
		{ 
			md5mesh[i].skeletonVert[j].Texcoord.x = submesh[i].vert[j].Texcoord.x;
			md5mesh[i].skeletonVert[j].Texcoord.y = submesh[i].vert[j].Texcoord.y;

			glm::vec4 temp = glm::vec4(0, 0, 0, 1);
			for (int m = 0; m < submesh[i].vert[j].num_weights; m++)
			{//单个顶点
				md5mesh[i].skeletonVert[j].Texcoord.x = submesh[i].vert[j].Texcoord.x;
				md5mesh[i].skeletonVert[j].Texcoord.y = submesh[i].vert[j].Texcoord.y;
				MD5weight& WEIGHT = submesh[i].weight[submesh[i].vert[j].first_weightindex + m];
				temp += joint[WEIGHT.joint_index].joint_transform * WEIGHT.weight_value * glm::vec4(WEIGHT.position, 1.0f);
			}
			md5mesh[i].skeletonVert[j].Vertice = vec4_to_vec3(temp);//生成顶点便于更好生成法线

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
		//法线
		for (int p = 0; p < submesh[i].numtriangulatefaces; p++)//多少个面
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
		md5mesh[i].Create_VAO_VBO_EBO_Buffer_Object();//先对每一个网格对象创建好各自的缓冲
		md5mesh[i].Bind_PerskeletonVertice_To_GPU();//绑定OpenGL的顶点数据
	}
	
}
void MD5FileData::Draw()const
{
	
	for (int i = 0; i < num_sub_meshes; i++)//遍历所有网格
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

