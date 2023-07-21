#version 330 core
in vec2 Texcoord;
in vec3 fragpos;
in vec3 Normal;
out vec4 frag_color;

uniform vec3 viewpos;
uniform vec3 lightpos;
uniform sampler2D Texbuffer0;
uniform sampler2D Texbuffer1;
void main()
{   

   //diffuse
   vec3 lightdir=normalize(lightpos-fragpos);
   float NdotL=max(dot(lightdir,Normal),0.2);
   vec4 diffuse=vec4(1,1,1,0)*NdotL;//法向量与光照方向余弦值


     //specular    
   vec3 viewdir=normalize(viewpos-fragpos);
   vec3 reflectdir=reflect(-lightdir,Normal);
   float rdotv=max(dot(reflectdir,viewdir),0.0f);
   vec4 specular=vec4(1,1,1,1)*pow(rdotv,120.0f)*12.8;//镜面反射光
   
  
   
     frag_color=mix(texture(Texbuffer0,Texcoord),texture(Texbuffer1,Texcoord),0.63)*(diffuse+ specular);
}