#version 330 core
in vec2 Texcoord;
in vec3 fragpos;
in vec3 Normal;
uniform vec3 viewpos;
uniform vec3 lightpos;
uniform sampler2D reflection_Texture;
uniform sampler2D floor_texture;
uniform sampler2D dudvmap;
out vec4 frag_color;
void main()
{
  
   vec3 lightdir=normalize(lightpos-fragpos);
   vec3 viewdir=normalize(viewpos-fragpos);
   vec3 reflectdir=reflect(-lightdir,Normal);
   float rdotv=max(dot(reflectdir,viewdir),0.0f);
   vec4 specular=vec4(1,1,1,0)*pow(rdotv,95)*120.8;//æµ√Ê∑¥…‰π‚
   frag_color=mix(texture(reflection_Texture,Texcoord),texture(floor_texture,Texcoord), 0.30432)+specular;

}