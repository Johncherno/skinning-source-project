#include "Drawcube.h"
#include<iostream>
void Drawcube::PushVerticesToGPU()
{
	PushourcubeVertice();
	glGenBuffers(1 ,&uu);
	glBindBuffer(GL_ARRAY_BUFFER, uu);
	glBufferData(GL_ARRAY_BUFFER, cubeMemoryVertices.size() * sizeof(glm::vec3), &cubeMemoryVertices[0], GL_STATIC_DRAW);


	glGenVertexArrays(1,&VAO);//����GPU���㻺�� VAO�ǵ�ַ
	glBindVertexArray(VAO);//VAO���㻺���GPU��ס

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);//���㲼��

}

void Drawcube::draw()
{
	glBindVertexArray(VAO);//VAO���㻺���GPU��ס
	glDrawArrays(GL_TRIANGLES, 0, cubeMemoryVertices.size());
	glBindVertexArray(0);//��󶥵��ַ
}

void Drawcube::PushourcubeVertice()
{//����������
	glm::vec3 cubeVertArray[36] = {
			//��������ǰ
      {-1.0f, 0.0f, -1.0f},
      {-1.0f, 2.0f, -1.0f},
      {1.0f, 2.0f, -1.0f},

    {-1.0f, 0.0f, -1.0f},
    {1.0f, 2.0f, -1.0f},
    {1.0f, 0.0f, -1.0f},

//����
//����������
         {-1.0f, 2.0f, -1.0f},
         {-1.0f, 2.0f, 1.0f},
         {1.0f, 2.0f, 1.0f},

{-1.0f, 2.0f, -1.0f},
{1.0f, 2.0f,   1.0f},
{1.0f, 2.0f,  -1.0f},

//�ұ�
//����������
{1.0f, 0.0f, -1.0f},
{1.0f, 2.0f, -1.0f},
{1.0f, 2.0f,  1.0f},

{1.0f, 0.0f, -1.0f},
{1.0f, 2.0f,  1.0f},
{1.0f, 0.0f,  1.0f},

//���
//����������
 {-1.0f, 0.0f, -1.0f},
 {-1.0f, 2.0f,  1.0f},
 {-1.0f, 2.0f, -1.0f},

 {-1.0f, 0.0f, -1.0f},
 {-1.0f, 0.0f,  1.0f},
 {-1.0f, 2.0f,  1.0f},

 //����
 //����������
 {-1.0f, 0.0f, 1.0f},
 {1.0f, 2.0f,  1.0f},
 {-1.0f, 2.0f, 1.0f},

 {-1.0f, 0.0f, 1.0f},
 {1.0f, 0.0f,  1.0f},
 {1.0f, 2.0f,  1.0f},

 //����
 //����������
 {-1.0f, 0.0f, -1.0f},
 {1.0f, 0.0f,   1.0f},
 {-1.0f, 0.0f,  1.0f},

 {-1.0f, 0.0f, -1.0f},
 {1.0f, 0.0f,  -1.0f},
 {1.0f, 0.0f,   1.0f}

	};
	
	for (glm::vec3 temp: cubeVertArray)
	{
		cubeMemoryVertices.push_back (temp);
	}
}
