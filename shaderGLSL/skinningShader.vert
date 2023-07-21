#version 330 core

layout(location=0)in vec3 position;
layout(location=1)in vec2 aTexcoord;
layout(location=2)in vec3 norm;
layout(location=3)in vec3 tangent;
layout(location=4)in vec4 boneIds;//layout(location=4)in ivec4 boneIds;�Ҿ�һֱ�������������
layout(location=5)in vec4 weights;

out vec2 Texcoord;
out vec3 Normal;
out vec3 fragpos;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform samplerBuffer  transformTex;//����任����



mat4 JointTransform(int jointindex)
{
        mat4 joint_transform;//���ڹ�����������任  ÿ�����ݲ����
        joint_transform[0]=texelFetch(transformTex,jointindex*4+0);//�����һ��
        joint_transform[1]=texelFetch(transformTex,jointindex*4+1);//����ڶ���
        joint_transform[2]=texelFetch(transformTex,jointindex*4+2);//���������
        joint_transform[3]=texelFetch(transformTex,jointindex*4+3);//���������
        return joint_transform;
}
void main()
{    
   

	 mat4 Bonetransform=mat4(1.0);
	   //weights[0]*JointTransform[boneIds[0]]*vert+weights[1]*JointTransform[boneIds[1]]*vert+weights[2]*JointTransform[boneIds[2]]*vert+weights[3]*JointTransform[boneIds[3]]*vert
	if(boneIds[0]!=-1)
	{   
	    Bonetransform=weights[0]*JointTransform(int(boneIds[0]));
	}
	if(boneIds[1]!=-1)
	{  
	    Bonetransform+=weights[1]*JointTransform(int(boneIds[1]));
	}
	if(boneIds[2]!=-1)
    {
	    Bonetransform+=weights[2]*JointTransform(int(boneIds[2]));
	}
	if(boneIds[3]!=-1)
	{  
	    Bonetransform+=weights[3]*JointTransform(int(boneIds[3]));
	}
   Texcoord=aTexcoord;//

 

   fragpos=(model *Bonetransform* vec4(position,1.0f)) .xyz;
   Normal=mat3(model*Bonetransform) *norm;
    gl_Position =  projection * view *  model *Bonetransform* vec4(position,1.0f);
}