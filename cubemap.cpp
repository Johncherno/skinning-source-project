#include"cubemap.h"
#include<iostream>
#include "gtc/matrix_transform.hpp"
#include"Texture2Dimage.h"
#include <stb_image.h>
void cubemap::loadtexture(
	std::string right ,
	std::string left ,
	std::string top ,
	std::string botttom ,
	std::string front ,
	std::string back )//加载天空盒图片
{
	
	hastexture = true;
	faces = { right ,left,top,botttom,front,back };


	glGenTextures(1,&id);
	glBindTexture(GL_TEXTURE_CUBE_MAP,id);

	int width, height, nchannels;
	for (int i=0;i<6;i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nchannels, 0);
		//std::cout <<faces[i].c_str()<<" " << data << std::endl;
		GLenum colormode = GL_RED;
		switch (nchannels)
		{
		case 3:
			colormode = GL_RGB;
			break;
		case 4:
			colormode = GL_RGBA;
			break;
		}
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,colormode,width,height,0,colormode,GL_UNSIGNED_BYTE,data);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
		
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	
}
void cubemap::init()
{
    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,//right

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,//left

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,//top

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,//bottom

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,//front

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f//back
    };
	glGenBuffers(1, &vbo);//给缓冲区分配地址
	glBindBuffer(GL_ARRAY_BUFFER, vbo);//绑定一个缓冲贮存区
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);//将顶点数据放到这个缓存区里

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);//顶点数据缓存地址及内存分配



	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, NULL); // 顶点数据缓存区每块的属性配置 字节大小
	glEnableVertexAttribArray(0);//position
    
}
void cubemap::render(glm::vec3&camera,glm::vec3& camerafront)
{   
	shaderProgram skyboxshader;
	skyboxshader.loadShaders("shaderGLSL//skybox.vert", "shaderGLSL//skybox.frag");
	glDepthMask(GL_FALSE);
	glm::mat4 model = glm::mat4(1.0f)*glm::scale(glm::mat4(1.0f),glm::vec3(23.0f,20.0f,20.0f));
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 targetpos(0,0,0);

	

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 0.0f, 1.0f) + camerafront, up);
	glm::mat4 projection = glm::perspective(glm::radians(42.0f), (float)(1024 / 1011), 0.003f, 100000.0f);
	skyboxshader.use();
	skyboxshader.setUniform("model", model);
	skyboxshader.setUniform("view", view);
	skyboxshader.setUniform("projection", projection);
	if (hastexture)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	}
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES,0,36);

	glDepthMask(GL_TRUE);
}