#include "mesh.h"
#include<iostream>
#include<sstream>
#include<fstream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include"filesystem.h"
std::vector<std::string> split(std::string s,std::string t)
{
	std::vector<std::string>res;
	while (1)
	{
		int pos = s.find(t);
		if (pos==-1)
		{
			res.push_back(s);
			break;
		}
		res.push_back(s.substr(0,pos));
		s = s.substr(pos+1,s.size()-pos-1);
	}
	return res;
}
bool mesh::loadobj(const std::string& filename)
{    
	std::vector<unsigned int> vertexIndieces, uvIndices,normalIndices;
	std::vector<glm::vec3> tempvertices;
	std::vector<glm::vec2> tempuvs;
	std::vector<glm::vec3> tempnormals;
	
	if (filename.find(".obj") != std::string::npos)
	{
		std::ifstream fin(filename, std::ios::in);
		if (!fin)
		{
			std::cout << "cannot open obj file" << filename << std::endl;

			return false;
		}
		std::cout << "loading obj file" << filename << ".........." << std::endl;
		std::string linebuffer;
		while (std::getline(fin, linebuffer))
		{
			
			std::stringstream ss(linebuffer);
			std::string cmd;
			ss >> cmd;
			if (cmd == "v")
			{
				
				glm::vec3 vertex;
				ss >> vertex.x; ss >> vertex.y; ss >> vertex.z;
				tempvertices.push_back(vertex);
				
			}
			else if (cmd == "vt")
			{

				
				glm::vec2 uv;
				ss >> uv.s; ss >> uv.t;
				tempuvs.push_back(uv);
			}
			else if (cmd == "vn")
			{

				
				glm::vec3 normals;
				ss >> normals.x; ss >> normals.y; ss >> normals.z;
				tempnormals.push_back(normals);
			}
			else if (cmd== "f")
			{
				
				std::string facedata;//字符流声明
				int vertexindex, uvindex, normalindex;//顶点数据索引数据
			     counter = 0;//面顶点计数器
				while (ss>>facedata)
				{
					
					counter++;//计数是否为3个顶点组成的三角形组成的面 还是四边形组成的面
						std::vector<std::string> data = split(facedata, "/");
						if (data[0].size()>0)
						{    
							 sscanf_s(data[0].c_str(), "%d", &vertexindex);
						     vertexIndieces.push_back(vertexindex);

						}
						if (data.size()>=1)
						{
							if (data[1].size() > 0)
							{  
								sscanf_s(data[1].c_str(), "%d", &uvindex);
						        uvIndices.push_back(uvindex);

							}
						}
						if (data.size() >= 2)
						{
							if (data[2].size() > 0)
							{
								sscanf_s(data[2].c_str(), "%d", &normalindex);
						        normalIndices.push_back(normalindex);

							}
						}	
				}
			}
		}
		
		

		for (unsigned int i = 0; i < vertexIndieces.size(); i++)
		{   vertex  meshVertex;
			if (tempvertices.size()>0)
			{
              glm::vec3 ver = tempvertices[vertexIndieces[i]-1 ];
              meshVertex.position = ver;
			}
			if (tempuvs.size() > 0)
			{
				glm::vec2 uv = tempuvs[uvIndices[i]-1];
				meshVertex.texcoord = uv;
			}
		   if (tempnormals.size() > 0)
		   {
			    glm::vec3 normal = tempnormals[normalIndices[i] - 1];
			    meshVertex.normal = normal;
		   }
			mvertices.push_back(meshVertex);
		
		}
		initbuffers();//将容器中的顶点  纹理坐标 法线赋给VAO
		


	}

}

void mesh::draw()
{    
	
	glBindVertexArray(vao);//储存三角形坐标值 绑定给着色器这些数据
   //switch (counter)
	//{
	//case 3:glDrawArrays(GL_TRIANGLES, 0, mvertices.size());//如果碰到的是三个点为一行就是三角形
	//case 4:glDrawArrays(GL_QUADS, 0, mvertices.size());//如果碰到的是四个点为一行就是四边形
	//}
    // glDrawArrays(GL_POINT, 0, mvertices.size());
	//glDrawArrays(GL_LINE_LOOP, 0, mvertices.size());//使能着色器开始绘制
	glDrawArrays(GL_TRIANGLES, 0, mvertices.size());
	
	//std::cout << vao << std::endl;
	 glBindVertexArray(0);
}
void mesh::meshdraw()
{
	glBindVertexArray(vao);//储存三角形坐标值
	glDrawArrays(GL_LINE_LOOP, 0, mvertices.size());
	glBindVertexArray(0);

	
}
void mesh::initbuffers()
{
	glGenBuffers(1, &vbo);//给缓冲区分配地址
	glBindBuffer(GL_ARRAY_BUFFER,vbo);//绑定一个缓冲贮存区
	glBufferData(GL_ARRAY_BUFFER, mvertices.size()*sizeof(vertex), &mvertices[0], GL_STATIC_DRAW);//将顶点数据放到这个缓存区里

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);//顶点数据缓存地址及内存分配


    glEnableVertexAttribArray(0);//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex) , NULL); // 顶点数据缓存区每块的属性配置 字节大小
	

    glEnableVertexAttribArray(1);//texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(GLfloat) * 3));
	
    glEnableVertexAttribArray(2);//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(GLfloat) * 5));
}