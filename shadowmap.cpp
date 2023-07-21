#include "shadowmap.h"
#include<GL/glew.h>
#include<GLFW/glfw3.h>
void shadowmap::Shadowmapinit()
{
	
	glGenFramebuffers(1, &shadowmapFBO);//帧缓冲创建
	glBindFramebuffer(GL_FRAMEBUFFER, shadowmapFBO);

	
	glGenTextures(1, &shadowmap);
	glBindTexture(GL_TEXTURE_2D, shadowmap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat boredercolor[] = {1.0f,1.0f,1.0f,1.0f};
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,boredercolor);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowmap, 0);	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void shadowmap::Shadowmapbindfbo()
{

	glViewport(0, 0, 4096, 4096);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowmapFBO);
}

glm::mat4 shadowmap::Shadowmapsetlight(glm::vec3& lightposition, float& left, float& right, float& bottom, float& top, float& near, float& far)
{
	lightP = glm::ortho(left, right, bottom, top, near, far);
	
	lightV = glm::lookAt(lightposition, glm::vec3(0, 0, 0), glm::vec3(0.0, 1.0, 0.0));
	lightspaceM = lightP * lightV;//光线视野矩阵
	return lightspaceM;
}

void shadowmap::setsamplershadowmap(shaderProgram* shader)
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowmap);
	glUniform1i(glGetUniformLocation(shader->getprogram(), "shadowmap"), 1);
}
