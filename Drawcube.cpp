#include "Drawcube.h"
#include<iostream>
void Drawcube::PushVerticesToGPU()
{
	PushourcubeVertice();
	glGenBuffers(1 ,&uu);
	glBindBuffer(GL_ARRAY_BUFFER, uu);
	glBufferData(GL_ARRAY_BUFFER, cubeMemoryVertices.size() * sizeof(glm::vec3), &cubeMemoryVertices[0], GL_STATIC_DRAW);


	glGenVertexArrays(1,&VAO);//生成GPU顶点缓存 VAO是地址
	glBindVertexArray(VAO);//VAO顶点缓存和GPU绑定住

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);//顶点布局

}

void Drawcube::draw()
{
	glBindVertexArray(VAO);//VAO顶点缓存和GPU绑定住
	glDrawArrays(GL_TRIANGLES, 0, cubeMemoryVertices.size());
	glBindVertexArray(0);//解绑顶点地址
}

void Drawcube::PushourcubeVertice()
{//立方体坐标
	glm::vec3 cubeVertArray[36] = {
			//法向量朝前
      {-1.0f, 0.0f, -1.0f},
      {-1.0f, 2.0f, -1.0f},
      {1.0f, 2.0f, -1.0f},

    {-1.0f, 0.0f, -1.0f},
    {1.0f, 2.0f, -1.0f},
    {1.0f, 0.0f, -1.0f},

//上面
//法向量朝上
         {-1.0f, 2.0f, -1.0f},
         {-1.0f, 2.0f, 1.0f},
         {1.0f, 2.0f, 1.0f},

{-1.0f, 2.0f, -1.0f},
{1.0f, 2.0f,   1.0f},
{1.0f, 2.0f,  -1.0f},

//右边
//法向量朝右
{1.0f, 0.0f, -1.0f},
{1.0f, 2.0f, -1.0f},
{1.0f, 2.0f,  1.0f},

{1.0f, 0.0f, -1.0f},
{1.0f, 2.0f,  1.0f},
{1.0f, 0.0f,  1.0f},

//左边
//法向量朝左
 {-1.0f, 0.0f, -1.0f},
 {-1.0f, 2.0f,  1.0f},
 {-1.0f, 2.0f, -1.0f},

 {-1.0f, 0.0f, -1.0f},
 {-1.0f, 0.0f,  1.0f},
 {-1.0f, 2.0f,  1.0f},

 //后面
 //法向量朝后
 {-1.0f, 0.0f, 1.0f},
 {1.0f, 2.0f,  1.0f},
 {-1.0f, 2.0f, 1.0f},

 {-1.0f, 0.0f, 1.0f},
 {1.0f, 0.0f,  1.0f},
 {1.0f, 2.0f,  1.0f},

 //下面
 //法向量朝下
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
