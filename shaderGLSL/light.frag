#version 330 core
in vec4 vertexColor; 
in vec2 TexCoord; 
out vec4 frag_Color;
in vec3  normal;
in vec3  fragpos;
uniform vec3 lightpos;
uniform vec3 viewpos;
uniform vec4 lightcolor;
uniform sampler2D image;
uniform float weight[5]=float[](0.2270270270,0.1945945946,0.1216216216,0.0540540541,0.0162162162);
void main()
{   
   vec2 tex_offset=1/textureSize(image,0);
   vec3 resultcolor=texture(image,TexCoord).rgb;
  for(int u=0;u<5;u++)
  {
      resultcolor+=texture(image,TexCoord+vec2(tex_offset.x*u,0.0f)).rgb;
      resultcolor+=texture(image,TexCoord-vec2(tex_offset.x*u,0.0f)).rgb;
  
  }
   frag_Color=lightcolor;//*vec4(resultcolor,1.0f);
 }