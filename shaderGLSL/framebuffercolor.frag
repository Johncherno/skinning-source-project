#version 330 core

struct Light{
     vec4 color;
     float shineness;
};
in vec4 vertexColor; 
in vec2 TexCoord; 
out vec4 frag_Color;
in vec3  normal;
in vec3  fragpos;
in vec4  Fragposlightspace;


uniform vec3 lightpos;
uniform vec3 viewpos;
uniform vec3 environmentlight;
uniform  sampler2D Texbuffer0;
uniform  sampler2D shadowmap;
uniform Light light;
float  attenuation;//��˥������
float distance;//���ڹ�˥���ľ������
float shadowcalc()//��Ӱ����  ��Ӱӳ��
{
   vec3 pos= (Fragposlightspace.xyz/Fragposlightspace.w)*0.5+0.5;
   if(pos.z>1.0)
   {
         pos.z=1.0;
    }
   float shadow = 0.0;
 vec2 texelSize = 1.0 / textureSize(shadowmap, 0);
 for(int x = -1; x <= 1; ++x)
 {
     for(int y = -1; y <= 1; ++y)
     {
         float pcfDepth = texture(shadowmap, pos.xy + vec2(x, y) * texelSize).r; 
         shadow += pos.z-0.05> pcfDepth  ? 0.0 : 1.0;        
     }    
 }
 shadow /= 9.0;
 return shadow;
 //float closetdepth=texture(shadowmap,pos.xy).r;
 //return pos.z>closetdepth?0:1;

}                                        //��Ӱ����
void main()
{   
   //diffuse
   vec3 pos= Fragposlightspace.xyz*0.5+0.5;
   vec3 lightdir=normalize(lightpos-fragpos);
   float NdotL=max(dot(lightdir,normal),0.2);
   vec4 diffuse=light.color*NdotL;//����������շ�������ֵ

  
   //ambient
   vec4 ambient=vec4(environmentlight,1.0);//������


   //specular    
   vec3 viewdir=normalize(viewpos-fragpos);
   vec3 reflectdir=reflect(-lightdir,normal);
   float rdotv=max(dot(reflectdir,viewdir),0.0f);
   vec4 specular=light.color*pow(rdotv,light.shineness)*12.8;//���淴���

   
   distance=length(vec3(lightpos-fragpos));//��ȡ��Դ��Ƭ�ξ���
   attenuation=200.0f/distance;//
 

   
 
   frag_Color=texture(Texbuffer0,TexCoord)*(shadowcalc()+ambient*0.8)*(diffuse+specular+ambient)*attenuation;
  
  
  
   
 }
 //��ɫ��ɫ��