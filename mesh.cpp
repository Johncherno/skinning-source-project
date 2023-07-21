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
				
				std::string facedata;//�ַ�������
				int vertexindex, uvindex, normalindex;//����������������
			     counter = 0;//�涥�������
				while (ss>>facedata)
				{
					
					counter++;//�����Ƿ�Ϊ3��������ɵ���������ɵ��� �����ı�����ɵ���
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
		initbuffers();//�������еĶ���  �������� ���߸���VAO
		


	}

}

void mesh::draw()
{    
	
	glBindVertexArray(vao);//��������������ֵ �󶨸���ɫ����Щ����
   //switch (counter)
	//{
	//case 3:glDrawArrays(GL_TRIANGLES, 0, mvertices.size());//�����������������Ϊһ�о���������
	//case 4:glDrawArrays(GL_QUADS, 0, mvertices.size());//������������ĸ���Ϊһ�о����ı���
	//}
    // glDrawArrays(GL_POINT, 0, mvertices.size());
	//glDrawArrays(GL_LINE_LOOP, 0, mvertices.size());//ʹ����ɫ����ʼ����
	glDrawArrays(GL_TRIANGLES, 0, mvertices.size());
	
	//std::cout << vao << std::endl;
	 glBindVertexArray(0);
}
void mesh::meshdraw()
{
	glBindVertexArray(vao);//��������������ֵ
	glDrawArrays(GL_LINE_LOOP, 0, mvertices.size());
	glBindVertexArray(0);

	
}
void mesh::initbuffers()
{
	glGenBuffers(1, &vbo);//�������������ַ
	glBindBuffer(GL_ARRAY_BUFFER,vbo);//��һ������������
	glBufferData(GL_ARRAY_BUFFER, mvertices.size()*sizeof(vertex), &mvertices[0], GL_STATIC_DRAW);//���������ݷŵ������������

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);//�������ݻ����ַ���ڴ����


    glEnableVertexAttribArray(0);//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex) , NULL); // �������ݻ�����ÿ����������� �ֽڴ�С
	

    glEnableVertexAttribArray(1);//texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(GLfloat) * 3));
	
    glEnableVertexAttribArray(2);//normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(GLfloat) * 5));
}