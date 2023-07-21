#version 330 core
layout(location=0) in vec3 pos; 
//layout(location=1) in vec3 color; 
layout(location=1) in vec2 aTexCoord;
layout(location=2) in vec3 anormal;

out vec2 TexCoord;
out vec4  vertexColor;
out vec3  normal;
out vec3  fragpos;
out vec4  Fragposlightspace;



uniform vec4 vercolor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightspacematrix;


void main()
{ 
  //gl_ClipDistance[0]=dot(vec4(fragpos,1.0f),plane);
 
   gl_Position=projection*view*model*vec4(pos,1.0f);
   vertexColor=vec4(1.0f,1.0f,1.0f,1.0f);
   TexCoord=aTexCoord;
   normal=vec3(model*vec4(anormal,0.0f));
   normal=normalize(normal);//一定要单位化这个法向量 因为要求的是角度值cos(theta)
   fragpos=vec3(model*vec4(pos,1.0f)); 
  
   Fragposlightspace=lightspacematrix*vec4(fragpos,1.0);
    
 }
 //顶点着色器
  