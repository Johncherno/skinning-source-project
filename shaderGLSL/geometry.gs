#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 3) out;
in vec3 normal[];
uniform float Time;
void Generateline(int index)
{
   gl_Position =gl_in[index].gl_Position;
   EmitVertex();
   gl_Position =gl_in[index].gl_Position+vec4(normal[index],0.0)*2.0;
    EmitVertex();
}
void main() {    

   vec3 a=vec3(gl_in[0].gl_Position)-vec3(gl_in[1].gl_Position);
   vec3 b=vec3(gl_in[2].gl_Position)-vec3(gl_in[1].gl_Position);
   vec3 Normal=normalize(cross(a,b));
 

 
    gl_Position =gl_in[0].gl_Position + vec4(Normal,1.0f); 
   
    EmitVertex();
  
    gl_Position =gl_in[1].gl_Position + vec4(Normal,1.0f); 
    
    EmitVertex();
    
    gl_Position = gl_in[2].gl_Position +vec4(Normal,1.0f); 
      
    EmitVertex();


  //Generateline(0);
  //  Generateline(1);
  //    Generateline(2);
   EndPrimitive();

}  