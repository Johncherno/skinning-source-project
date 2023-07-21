#version 330 core
layout(location=0) in vec3 pos; 
layout(location=1) in vec3 color; 
uniform vec4 vertcolor;
out vec4 vertexColor; 
void main()
{
 gl_Position=vec4(pos.x,pos.y,pos.z,1.0);
 vertexColor=vec4(color,1.0f);
}

 
  