#ifndef MESH_H
#define MESH_H
#include<vector>
#include<string>
#include"GL/glew.h"
#include"glm.hpp"
struct vertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
	glm::vec3 tangent;
};
class mesh
{
public:
	bool loadobj(const std::string& filename);
	void draw();
	void meshdraw();
private: 
	int counter;//�����Ƕ��ٸ���Ϊһ����
	void initbuffers();
	bool mloaded;
	std::vector<vertex>mvertices;
	GLuint vbo;GLuint  vao;
	
};
#endif
