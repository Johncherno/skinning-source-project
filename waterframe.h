#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>
class water
{
public:void init()
	{
		//绑定水面帧缓冲
		glGenFramebuffers(1, &waterframeBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, waterframeBuffer);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);//绑定水面帧缓冲



	
		glGenTextures(1, &reflection_texture);
		glBindTexture(GL_TEXTURE_2D, reflection_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflection_texture, 0);//在这个帧缓冲下绑定一张颜色纹理贴图用于刻录当前
	}
	unsigned int waterframebuffer() { return waterframeBuffer; }
	unsigned int reflectiontexture() { return reflection_texture; }
private:
	unsigned int waterframeBuffer;
	unsigned int reflection_texture;
};