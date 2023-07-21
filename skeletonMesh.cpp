#include "skeletonMesh.h"
#include<iostream>
Mesh::Mesh(std::vector<SKeleTonMeshVertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
	setupmesh();
}

void Mesh::Draw()//void BasicMesh::Render()
{
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}
void Mesh::setupmesh()
{
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(SKeleTonMeshVertex)*vertices.size(),&vertices[0],GL_STATIC_DRAW);

	glGenBuffers(1,&EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);//索引

	glEnableVertexAttribArray(0);//position                                                                                         
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(SKeleTonMeshVertex),  (GLvoid*)offsetof(SKeleTonMeshVertex, position));
                      
	glEnableVertexAttribArray(1);//texcoord														  
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, sizeof(SKeleTonMeshVertex),  (GLvoid*)offsetof(SKeleTonMeshVertex, texcoord));

	glEnableVertexAttribArray(2);//normal																
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(SKeleTonMeshVertex),  (GLvoid*)offsetof(SKeleTonMeshVertex,   normal));
	
	glEnableVertexAttribArray(3);//tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, sizeof(SKeleTonMeshVertex),  (GLvoid*)offsetof(SKeleTonMeshVertex,   Tangent));

	glEnableVertexAttribArray(4);//boneIDs  骨骼索引
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(SKeleTonMeshVertex),  (GLvoid*)offsetof(SKeleTonMeshVertex, BoneTranSformMatrixIndex));
	
	glEnableVertexAttribArray(5);//weights   骨骼权重
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_TRUE, sizeof(SKeleTonMeshVertex),  (GLvoid*)offsetof(SKeleTonMeshVertex,  m_Weights));
	
	 
}
