#version 330 core

layout(location=0)in vec3 position;
layout(location=1)in vec2 aTexcoord;
layout(location=2)in vec3 anormal;
layout(location=3)in vec2 weighting;//这个顶点x  数量 y第一个索引

out vec2 Texcoord;
out vec3 Normal;
out vec3 fragpos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform samplerBuffer weightTex;//权重结构体
uniform samplerBuffer  transformTex;//顶点变换矩阵

void main(void)
{
    
     

     int this_glVertices_num_weights 	= int(weighting.x);	 //这个顶点的权重数量
	 int this_glVertices_first_index	= int(weighting.y);	
     vec4 vertPos 	= vec4(0,0,0,1); 
     vec4 temp_normal=vec4(0,0,0,1);
     for(int i=0;i<this_glVertices_num_weights;i++)
     {
         //weight 819 24 0.200000003 ( 6.2169904709 -10.885181427 11.7152395248 )

        vec3 first_half=texelFetch(weightTex,(this_glVertices_first_index +i)*2).rgb;//第一行 每个内容差两行  (this_glVertice_first_index+i)*2-> 819    24    0.200000003// r g b//weightindex  jointindex  weightvalue
        
        vec3 weight_position=texelFetch(weightTex,((this_glVertices_first_index +i)*2)+1).rgb; // (this_glVertice_first_index+i)*2+1->( 6.2169904709    -10.885181427    11.7152395248 )// r g b//weightpos.x  weightpos.y  weightpos.z   
       

        int jointindex =int(first_half.y);
       float weightvalue=(first_half.z);

        mat4 joint_transform;//用于骨骼顶点坐标变换  每个内容差八行
       joint_transform[0]=texelFetch(transformTex,jointindex*4+0);//矩阵第一行
       joint_transform[1]=texelFetch(transformTex,jointindex*4+1);//矩阵第二行
       joint_transform[2]=texelFetch(transformTex,jointindex*4+2);//矩阵第三行
       joint_transform[3]=texelFetch(transformTex,jointindex*4+3);//矩阵第四行


     
        //顶点
      
      vertPos+=weightvalue* joint_transform*vec4(weight_position,1.0);
      //法线    
    
      temp_normal+=weightvalue*inverse(joint_transform)*vec4(anormal,1.0f);
     }
       
    Normal = normalize(vec3(model* temp_normal));//法线输出
    Texcoord=aTexcoord;//纹理坐标
    fragpos=vec3(model*vertPos);
    gl_Position	=projection*view*model* vec4( vertPos.xyz,1);
}