#include "MD5mesh.h"
void MD5mesh::Bind_PerskeletonVertice_To_GPU()
{
	
	
	glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SkeletonVertex) * num_vertices,&skeletonVert[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, &indices[0], GL_STATIC_DRAW);//Ë÷Òý

	glEnableVertexAttribArray(0);//position                                                                                         
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletonVertex), NULL);


	glEnableVertexAttribArray(1);//texture															  
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SkeletonVertex), (GLvoid*)(sizeof(GLfloat) * 3));
														   
	glEnableVertexAttribArray(2);//normal															
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SkeletonVertex), (GLvoid*)(sizeof(GLfloat) * 5));
														   
	glEnableVertexAttribArray(3);//weighting			   
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(SkeletonVertex), (GLvoid*)(sizeof(GLfloat) * 8));
}

void MD5mesh::Create_VAO_VBO_EBO_Buffer_Object()
{

	glGenVertexArrays(1, &VAO);
	

	glGenBuffers(1, &VBO);


	glGenBuffers(1, &EBO);

}
void MD5mesh::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
