#version 330 core
layout(location=0) in vec3 pos; 
layout(location=2) in vec3 anormal;



out vec3  normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;




void main()
{ 
  
 
   gl_Position=projection*view*model*vec4(pos,1.0f);
  
   normal=vec3(model*vec4(anormal,0.0f));
   normal=normalize(normal);//一定要单位化这个法向量 因为要求的是角度值cos(theta)
 
 }
 //顶点着色器