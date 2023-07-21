#version 330 core 
in vec3 TexCoords; 
out vec4 frag_Color;

uniform samplerCube skybox;


void main()
{
   frag_Color=texture(skybox,TexCoords);
}