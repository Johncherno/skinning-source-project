#version 330 core

layout(location=0)in vec3 position;
layout(location=1)in vec2 aTexcoord;
layout(location=2)in vec4 boneIds;
layout(location=3)in vec4 weights;
out vec4 pos;
uniform mat4 finaltranform[2];
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(void)
{
   mat4 Bonetransform=mat4(1.0);
	   
	  
	

	if(boneIds[0]!=-1)
	{   
	    Bonetransform=weights[0]*finaltranform[int(boneIds[0])];
	}
	if(boneIds[1]!=-1)
	{  
	    Bonetransform+=weights[1]*finaltranform[int(boneIds[1])];
	}
    if(boneIds[0]==-1&&boneIds[1]==-1)
	{
	  Bonetransform=mat4(1.0);
	}
	pos=Bonetransform* vec4(position,1.0f);

  gl_Position=projection*view*model*Bonetransform* vec4(position,1.0f);
}
