#version 330 core
layout(location=0) in vec3 pos; 
layout(location=1) in vec2 aTexCoord;
layout(location=2) in vec3 anormal;
uniform vec4 vercolor;
out vec2 TexCoord;
out vec4  vertexColor;
out vec3  normal;
out vec3  fragpos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{  
   gl_Position=projection*view*model*vec4(pos,1.0f);
   vertexColor=vec4(1.0f,1.0f,1.0f,1.0f);
   TexCoord=aTexCoord;
   
 }

  