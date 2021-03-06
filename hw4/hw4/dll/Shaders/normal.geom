#version 430 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform mat4 P;
uniform float tc;
uniform float falling;

out vec4 color;

void triangle(int index)
{
    float timer = tc;
    vec4 drop = vec4(gs_in[index].normal, 0.0f) * 0.25 + vec4(0.0f,-falling,0.0f,0.0f);
    if(falling != 0){
        drop *= falling;
        timer = 3.1425;
    }

    color=vec4(0.0,0.0,1.0,1.0);
    gl_Position = gl_in[index].gl_Position + drop*sin(timer);
    EmitVertex();
    
    color=vec4(0.0,1.0,0.0,1.0);
    gl_Position = gl_in[index+1].gl_Position + drop*sin(timer);
    EmitVertex();

    color=vec4(1.0,0.0,0.0,1.0);
    gl_Position = gl_in[index+2].gl_Position + drop*sin(timer);
    EmitVertex();
    
    EndPrimitive();
}

void main()
{
    for(int i=0;i<gl_in.length();i+=3){
        triangle(i);
    }
}
