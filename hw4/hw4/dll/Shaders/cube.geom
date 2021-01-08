#version 430 core
layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

uniform mat4 P;
uniform float tc;
uniform float falling;

out vec4 color;

void triangle(int index)
{   
    color=vec4(0.0,0.0,1.0,1.0);
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    
    color=vec4(0.0,1.0,0.0,1.0);
    gl_Position = gl_in[index+1].gl_Position;
    EmitVertex();

    color=vec4(1.0,0.0,0.0,1.0);
    gl_Position = gl_in[index+2].gl_Position;
    EmitVertex();
    
    EndPrimitive();
}

void main()
{
    for(int i=0;i<gl_in.length();i+=3){
        triangle(i);
    }
}
