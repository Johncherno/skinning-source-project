#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<string>
#include<glm.hpp>
#include"gtc/matrix_transform.hpp"
#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include"MD5mesh.h"
//MD5file->CreateTBO()创建纹理缓冲
//先更新每个关节矩阵   skeleton->transformjoint(time);
//   再  MD5file->BindTextureBuffers();//绑定纹理缓冲
//  再   MD5file->update_transformTBO( skeleton);   把skeleton已经更新好的矩阵放入这个数据矩阵纹理缓冲texture一下
//animationshader.use()
//glUniform1i(glGetUniformLocation(animationshader->GetProgram(), "weightTex"), 0);
//glUniform1i(glGetUniformLocation(animationshader->GetProgram(), "transformTex"),1);
//hellnight.Draw();
struct MD5vert//顶点
{
	int vert_index;//顶点坐标索引
	glm::vec2 Texcoord;//纹理坐标
	int first_weightindex;//这个顶点对应的在mesh里第一个权重值索引
	int num_weights;//这个顶点到底绕多少个骨骼在转 对映的权重值个数
};
struct MD5weight//绕着某一特定骨骼旋转的权重值
{
	int weight_index;//权重值索引
	int joint_index;//骨骼节点索引 方便访问它的变换矩阵
	float weight_value;//权重数值
	glm::vec3 position;//三维位置顶点坐标
};
struct MD5triangulte_face//三角形面
{
	unsigned int triangulte_face_index;//三角形面索引
	unsigned int first_vertice_index;//三角形面的第几个点
	unsigned int second_vertice_index;
	unsigned int third_vertice_index;
};
struct MD5subMesh
{
	int numverts;//多少个顶点
	int numweights;//多少个权重结构体
	int numtriangulatefaces;//多少个三角形面
	MD5vert* vert;//生成顶点数组
	MD5weight* weight;//生成权重结构体数组
	MD5triangulte_face* triangular_face;//生成三角形面数组
};//MD5网格类
struct MD5joint//骨骼节点
{
	std::string name;//这个骨骼关节名字
	int parent_index;//父节点
	glm::vec3 position;//要安放在这个模型的位置
	glm::vec3 orientation;//欧拉角
	glm::mat4 accumulate_transform;//绕着世界坐标系的矩阵
	glm::mat4 joint_transform;//这个关节的变换矩阵
	glm::mat4 initial_Joint_Matrix;
};
struct MD5Skeleton {
	MD5joint* joints;	//骨骼关节数组
	int numJoints;		//Number of joints in the skeleton
};
struct initialpos
{
	glm::vec3* position;//一系列骨骼节点初始位置和旋转角
	glm::vec3* orientation;
};
class MD5FileData
{
public:
	friend class MD5Node;//用MD5Node来访问私有函数
	
	MD5FileData(const std::string& filename);
	void Draw() const;
   
protected:
	void loadMesh( std::ifstream&from,int &Meshcount);
	void CreateMesh();
	void CreateTBO();
    void BindTextureBuffers()const;//绑定纹理缓冲
	void update_transformTBO(MD5Skeleton&skeleton)const;
	void clone_skeletoninformation(MD5Skeleton& destination_skeleton)const;//将自己类中的骨骼节点欣喜复刻到目标地点
	glm::vec3 vec4_to_vec3(glm::vec4&temp);
	glm::mat4 toRotation(glm::vec3&p);
	int	LoadMD5Joints(std::ifstream& from);//加载骨骼关节
	int num_sub_meshes;
	int num_joints;
	MD5subMesh *submesh;
	MD5joint *joint;//骨骼数组
	MD5mesh*md5mesh;
	initialpos  initpos;
	GLuint weightbuffer;//权重缓冲
	GLuint transformbuffer;//矩阵缓冲
	GLuint weightTexture;//权重纹理
	GLuint transformTexture;//矩阵纹理
	glm::vec3 *weighting;
	glm::mat4 *Transform;
};

