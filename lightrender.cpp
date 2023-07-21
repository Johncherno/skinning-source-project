#include "lightrender.h"
#include "gtc/matrix_transform.hpp"
void lightrender::init()
{
	
	lightshader.loadShaders("shaderGLSL\\light.vert", "shaderGLSL\\light.frag");//�ƹ�λ����Ⱦ
	lightmesh.loadobj("mesh\\sphere.obj");

	glGenFramebuffers(1,&BlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,BlurFBO);
	glGenTextures(1,&colorbuffer);
	glBindTexture(GL_TEXTURE_2D,colorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1920, 1080, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,colorbuffer,0);
	//create and attach depthbuffer
	unsigned int rbodepth;
	glGenRenderbuffers(1,&rbodepth);
	glBindRenderbuffer(GL_RENDERBUFFER,rbodepth);
	glRenderbufferStorage(GL_RENDER,GL_DEPTH_ATTACHMENT,1920,1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rbodepth);
	//������Щ��ɫö����
	unsigned int attachment[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1,attachment);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void lightrender::lightblurbindFBO()
{
	glViewport(0, 0, 1920, 1080);
	glBindFramebuffer(GL_FRAMEBUFFER,BlurFBO);//��֡����
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void lightrender::render(glm::vec3&camera,glm::vec3&lightpos,glm::vec4&lightcolor, glm::vec3& camerafront)
{
	glm::mat4  view;//��Ұ����  ͶӰ����
	glm::mat4 model = glm::mat4(1.0f);//��ʼ��ģ�;���
	glm::vec3 targetpos;//Ŀ��
	glm::vec3 up;
	targetpos = glm::vec3(0,0,1);
	up = glm::vec3(0,1,0);
	
	lightshader.use();
	glm::mat4 projection = glm::perspective(glm::radians(35.0f), (float)(1024 / 1011), 0.3f, 100000.0f);
	lightshader.setUniform("projection", projection);
	view = glm::lookAt(camera, camera+camerafront, up);//��Ұ����
	lightshader.setUniform("view", view);
	model = glm::scale(glm::mat4(1.0f),glm::vec3(7,7,7));
	model = glm::translate(model, lightpos) * glm::rotate(model, glm::radians(43.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(32.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(39.0f), glm::vec3(1.0f, 0.0f, 0.0f));//transform
	lightshader.setUniform("model", model);
	lightshader.setUniform("lightcolor", lightcolor);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorbuffer);
	glUniform1i(glGetUniformLocation(lightshader.getprogram(), "image"), 0);
	lightmesh.draw();//��ʼ��Ⱦ�ƹ�
	
}
