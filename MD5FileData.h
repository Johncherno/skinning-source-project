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
//MD5file->CreateTBO()����������
//�ȸ���ÿ���ؽھ���   skeleton->transformjoint(time);
//   ��  MD5file->BindTextureBuffers();//��������
//  ��   MD5file->update_transformTBO( skeleton);   ��skeleton�Ѿ����ºõľ������������ݾ���������textureһ��
//animationshader.use()
//glUniform1i(glGetUniformLocation(animationshader->GetProgram(), "weightTex"), 0);
//glUniform1i(glGetUniformLocation(animationshader->GetProgram(), "transformTex"),1);
//hellnight.Draw();
struct MD5vert//����
{
	int vert_index;//������������
	glm::vec2 Texcoord;//��������
	int first_weightindex;//��������Ӧ����mesh���һ��Ȩ��ֵ����
	int num_weights;//������㵽���ƶ��ٸ�������ת ��ӳ��Ȩ��ֵ����
};
struct MD5weight//����ĳһ�ض�������ת��Ȩ��ֵ
{
	int weight_index;//Ȩ��ֵ����
	int joint_index;//�����ڵ����� ����������ı任����
	float weight_value;//Ȩ����ֵ
	glm::vec3 position;//��άλ�ö�������
};
struct MD5triangulte_face//��������
{
	unsigned int triangulte_face_index;//������������
	unsigned int first_vertice_index;//��������ĵڼ�����
	unsigned int second_vertice_index;
	unsigned int third_vertice_index;
};
struct MD5subMesh
{
	int numverts;//���ٸ�����
	int numweights;//���ٸ�Ȩ�ؽṹ��
	int numtriangulatefaces;//���ٸ���������
	MD5vert* vert;//���ɶ�������
	MD5weight* weight;//����Ȩ�ؽṹ������
	MD5triangulte_face* triangular_face;//����������������
};//MD5������
struct MD5joint//�����ڵ�
{
	std::string name;//��������ؽ�����
	int parent_index;//���ڵ�
	glm::vec3 position;//Ҫ���������ģ�͵�λ��
	glm::vec3 orientation;//ŷ����
	glm::mat4 accumulate_transform;//������������ϵ�ľ���
	glm::mat4 joint_transform;//����ؽڵı任����
	glm::mat4 initial_Joint_Matrix;
};
struct MD5Skeleton {
	MD5joint* joints;	//�����ؽ�����
	int numJoints;		//Number of joints in the skeleton
};
struct initialpos
{
	glm::vec3* position;//һϵ�й����ڵ��ʼλ�ú���ת��
	glm::vec3* orientation;
};
class MD5FileData
{
public:
	friend class MD5Node;//��MD5Node������˽�к���
	
	MD5FileData(const std::string& filename);
	void Draw() const;
   
protected:
	void loadMesh( std::ifstream&from,int &Meshcount);
	void CreateMesh();
	void CreateTBO();
    void BindTextureBuffers()const;//��������
	void update_transformTBO(MD5Skeleton&skeleton)const;
	void clone_skeletoninformation(MD5Skeleton& destination_skeleton)const;//���Լ����еĹ����ڵ���ϲ���̵�Ŀ��ص�
	glm::vec3 vec4_to_vec3(glm::vec4&temp);
	glm::mat4 toRotation(glm::vec3&p);
	int	LoadMD5Joints(std::ifstream& from);//���ع����ؽ�
	int num_sub_meshes;
	int num_joints;
	MD5subMesh *submesh;
	MD5joint *joint;//��������
	MD5mesh*md5mesh;
	initialpos  initpos;
	GLuint weightbuffer;//Ȩ�ػ���
	GLuint transformbuffer;//���󻺳�
	GLuint weightTexture;//Ȩ������
	GLuint transformTexture;//��������
	glm::vec3 *weighting;
	glm::mat4 *Transform;
};

